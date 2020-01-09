import hashlib
import socket
import threading
import struct
from enum import IntEnum
import QQSQL
import base64

class DataType(IntEnum):
    SYS = 0   # 系统信息
    LOGIN = 2  # 登录消息
    REGISTE = 3  # 注册消息
    FRIENDLIST = 4  # 刷新出好友列表
    ADDFRIEND = 5  # 好友申请
    ADDFRIENDCOMFIRM = 6  # 好友申请回复
    SENDMESSAGE = 7  # 发送信息
    SEARCHDATA = 8
    CREATEGROUP = 9  # 创建群聊
    GROUPLIST = 10 # 刷新组表
    ADDGROUP = 11  # 加入群聊
    GROUPMEMBERFLUSH = 12
    SENDGROUPMESSAGE = 13
    SEARCHGROUPDATA = 14


def md5(s):
    hashObj = hashlib.md5()
    hashObj.update(s.encode('gb2312'))
    return hashObj.hexdigest()


class Handler:
    def __init__(self):
        self.user_id = None
        self.args = None
        self.socket = None

    # 注册
    def REGISTE(self):
        # 检查账号是否注册了
        sql = "select * from user_info where user_id='%s';" % (self.args[0])
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        result = _db.cursor.fetchone()
        if result is not None:
            sendtoclientsocket(self.socket, DataType.REGISTE, 0, "用户已存在")
            return
        # 保存账号和密码到数据库
        passwd = md5(self.args[1])
        # 一开始不在线
        sql = "INSERT INTO user_info(user_id, user_pswd, isOnline) VALUES('%s', '%s', %d);" % (self.args[0], passwd, 0)
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        _db.cursor.execute('commit;')  # 提交到数据库
        lock.release()
        sendtoclientsocket(self.socket, DataType.REGISTE, 1, "注册成功")

    # 登录
    def LOGIN(self):
        # 查表，判断用户是否存在
        sql = "select * from user_info where user_id = '%s';" % (self.args[0])
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        userInfo = _db.cursor.fetchone()
        if userInfo is None:
            sendtoclientsocket(self.socket, DataType.LOGIN, 0, "账号不存在")
            return
        # 比较密码是否正确
        if userInfo['user_pswd'] != md5(self.args[1]):
            sendtoclientsocket(self.socket, DataType.LOGIN, 0, "密码错误")
            return
        if int(self.args[1]) in _clientDict.keys():
            sendtoclientsocket(self.socket, DataType.LOGIN, 0, "已经登陆过了")
            return
        sendtoclientsocket(self.socket, DataType.LOGIN, 1, "密码正确")
        # 将已登录的用户信息保存到字典里
        self.__dict__.update(userInfo)
        _clientDict[self.user_id] = self

    # 获取好友列表
    def FRIENDLIST(self):
        sql = "select user_id2 from friendship_info where user_id1 = '%s';" % (self.args[0])
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        result = _db.cursor.fetchall()
        global _FriendDict
        _FriendDict = result
        data = str(len(result))  # 先发送好友数量给客户端
        for i in result:
            data += '\n'
            data += str(i['user_id2'])
        sendtoclientsocket(self.socket, DataType.FRIENDLIST, 1, data)

    # 好友申请
    def ADDFRIEND(self):
        # 1.1 判断要添加的用户账号是否存在
        sql = "select * from user_info where user_id ='%s';" % (self.args[1])
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        user = _db.cursor.fetchone()
        if user == None:
            sendtoclientsocket(self.socket, DataType.ADDFRIEND, 0, "要添加的用户不存在")
            return
            # 2. 判断是否已经添加过了
        sql = "select * from friendship_info where user_id1= '%s' and user_id2= '%s';" % (self.args[0], self.args[1])
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        # 获取查询的结果集,如果啥都没查到就返回None,否则就不会是None
        if _db.cursor.fetchone() is not None:
            sendtoclientsocket(self.socket, DataType.ADDFRIEND, 0, "已添加过了")
            return
        if not int(self.args[1]) in _clientDict.keys():
            sendtoclientsocket(_clientDict[int(self.args[0])].socket, DataType.ADDFRIEND, 0, "对方不在线")
        else:
            # 将通知发送给要添加的好友
            str = "%s想要添加你为好友" % self.args[0]
            sendtoclientsocket(_clientDict[int(self.args[1])].socket, DataType.ADDFRIEND, 1, str)  # 状态码1 代表了是客户端发过来的

    # 好友申请的回复
    def ADDFRIENDCOMFIRM(self):
        if self.args[2] == "ERR":
            sendtoclientsocket(_clientDict[int(self.args[0])].socket, DataType.ADDFRIEND, 0,
                               "对方已拒绝")  # 状态码0 代表了是服务器发给客户端
        elif self.args[2] == "ADDFRIENDCOMFIRM":
            sendtoclientsocket(_clientDict[int(self.args[0])].socket, DataType.ADDFRIEND, 0, "已成功添加对方为好友")
            # 发送信息以后，就要插入到数据库了
            sql1 = "INSERT INTO friendship_info(user_id1, user_id2) VALUES('%s', '%s');" % (self.args[0], self.args[1])
            sql2 = "INSERT INTO friendship_info(user_id1, user_id2) VALUES('%s', '%s');" % (self.args[1], self.args[0])
            _db.cursor.execute(sql1)
            _db.cursor.execute(sql2)
            _db.cursor.execute('commit;')  # 提交到数据库

    # 发送消息
    def SENDMESSAGE(self):
        # 将消息传给数据库
        # user_id是发信息者的id
        # user_id2是收信息者的id
        user_id2 = self.args[0]
        sql = "INSERT INTO chat_record(user_id1, user_id2, record) VALUES('%s', '%s', '%s');" % (self.user_id, user_id2,
                                                                                                 self.args[1])
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        _db.cursor.execute('commit')
        lock.release()
        # 告诉对方有信息
        sendtoclientsocket(self.socket, DataType.SENDMESSAGE, 0, str(user_id2))  # 发送给自己
        if int(self.args[0]) in _clientDict:  # 如果对方在线的话，那么就发消息给他
            sendtoclientsocket(_clientDict[int(self.args[0])].socket, DataType.SENDMESSAGE, 0, str(self.user_id))  # 发送给对方

    # 搜索聊天记录
    def SEARCHDATA(self):
        user_id2 = self.args[0]
        sql = "SELECT record FROM chat_record WHERE (user_id1 = '%s' AND user_id2 = '%s') OR (user_id1 = '%s' AND " \
              "user_id2 = '%s') " % (self.user_id, user_id2, user_id2, self.user_id)  # 进行查询
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        result = _db.cursor.fetchall()
        data = user_id2
        data += '\n'
        data += str(len(result))  # 先发送聊天信息数量给客户端
        for i in result:
            data += '\n'
            data += str(i['record'])
        if self.args[1] == "record":  #如果第二个参数是record的话
            sendtoclientsocket(self.socket, DataType.SEARCHDATA, 0, data)  # 此时0代表的是查看记录框的更新
        else:
            sendtoclientsocket(self.socket, DataType.SEARCHDATA, 1, data)   # 此时1代表的是聊天框的更新

    # 创建群聊
    def	CREATEGROUP(self):
        sql = "SELECT * FROM room_info WHERE room_id = '%s'" % self.args[0]  # 查询房间是否存在了
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()  # 释放
        result = _db.cursor.fetchone()
        if result is not None:
            sendtoclientsocket(self.socket, DataType.SYS, 1, "房间已存在了")
            return
        # 否则创建新的房间，插入数据库
        lock.acquire()  # 获取锁
        sql = "INSERT INTO room_info(room_id, boss) VALUES('%s', '%s')" % (self.args[0], self.user_id)
        _db.cursor.execute(sql)
        sql = "INSERT INTO room_member(room_id, user_id) VALUES('%s','%s')" % (self.args[0], self.user_id)
        _db.cursor.execute(sql)
        _db.cursor.execute('commit;')
        lock.release()  # 释放
        sendtoclientsocket(self.socket, DataType.SYS, 1, "成功")

    # 刷新群列表
    def GROUPLIST(self):
        sql = "SELECT room_id FROM room_member WHERE user_id = '%s'" % self.user_id  # 查看有什么房间
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        result = _db.cursor.fetchall()
        global _GroupDict
        _GroupDict = result
        data = str(len(result))  # 先发送好友数量给客户端
        for i in result:
            data += '\n'
            data += str(i['room_id'])
        sendtoclientsocket(self.socket, DataType.GROUPLIST, 1, data)

    # 加入群聊
    def ADDGROUP(self):
        lock.acquire()  # 获取锁
        sql = "SELECT * FROM room_info WHERE room_id = '%s'" % self.args[0]  # 查看房间是否存在
        _db.cursor.execute(sql)
        lock.release()  # 释放
        result = _db.cursor.fetchone()
        if result is None:
            sendtoclientsocket(self.socket, DataType.SYS, 1, "房间不存在")
            return
        lock.acquire()  # 获取锁
        sql = "SELECT * FROM room_member WHERE room_id = '%s' AND user_id = '%s'" % (self.args[0], self.user_id)  # 查看是否已经加入了
        _db.cursor.execute(sql)
        lock.release()  # 释放
        result = _db.cursor.fetchone()
        if result is not None:
            sendtoclientsocket(self.socket, DataType.SYS, 1, "已经加入房间了")
            return
        lock.acquire()  # 获取锁
        sql = "INSERT INTO room_member(room_id, user_id) VALUES('%s','%s')" % (self.args[0], self.user_id)
        _db.cursor.execute(sql)
        lock.release()
        sendtoclientsocket(self.socket, DataType.SYS, 1, "加入成功")

    # 刷新群友列表
    def GROUPMEMBERFLUSH(self):
        lock.acquire()  # 获取锁
        sql = "SELECT user_id FROM room_member WHERE room_id = '%s'" % self.args[0]
        _db.cursor.execute(sql)
        lock.release()  # 释放
        result = _db.cursor.fetchall()
        global _GroupMember
        _GroupMember = result
        data = self.args[0]     # 窗口的房间号
        data += '\n'
        data += str(len(result))  # 发送好友数量给客户端
        for i in result:
            data += '\n'
            data += str(i['user_id'])
        sendtoclientsocket(self.socket, DataType.GROUPMEMBERFLUSH, 1, data)

    # 发送群聊信息
    def SENDGROUPMESSAGE(self):
        # 将消息传给数据库
        # args[0]是房间号
        # args[1]是信息
        room_id = self.args[0]
        sql = "INSERT INTO room_record(room_id, user_id, record) VALUES('%s', '%s', '%s');" % (room_id,
                                                                                                    self.user_id,
                                                                                                    self.args[1])
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        _db.cursor.execute('commit')
        lock.release()
        sendtoclientsocket(self.socket, DataType.SENDMESSAGE, 1, str(room_id))  # 将房间号码发送给自己  # status为1代表发送给群聊
        # 告诉群友有信息
        sql = "SELECT user_id FROM room_member WHERE room_id = '%s';" % room_id
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        result = _db.cursor.fetchall()
        for i in result:
            if int(i['user_id']) in _clientDict:  # 如果对方在线的话，那么就发消息给他
                sendtoclientsocket(_clientDict[int(i['user_id'])].socket, DataType.SENDMESSAGE, 1,
                                   str(room_id))  # 发送给对方

    # 搜索群聊的数据库
    def SEARCHGROUPDATA(self):
        room_id = self.args[0]   # 房间号
        sql = "SELECT record FROM room_record WHERE (room_id = '%s')" %  room_id  # 进行查询
        lock.acquire()  # 获取锁
        _db.cursor.execute(sql)
        lock.release()
        result = _db.cursor.fetchall()
        data = room_id
        data += '\n'
        data += str(len(result))  # 先发送聊天信息数量给客户端
        for i in result:
            data += '\n'
            data += str(i['record'])
        if self.args[1] == "record":  # 如果第二个参数是record的话
            sendtoclientsocket(self.socket, DataType.SEARCHGROUPDATA, 0, data)  # 此时0代表的是查看记录框的更新
        else:
            sendtoclientsocket(self.socket, DataType.SEARCHGROUPDATA, 1, data)  # 此时1代表的是聊天框的更新

# 发送给客户端的
def sendtoclientsocket(client, Type, status, strData):
    data = strData.encode('gb2312')
    size = len(data)
    strFormat = 'iii%ds' % size
    rawData = struct.pack(strFormat, Type.value, status, size, data)
    client.send(rawData)

# 从客户端那里接收信息
def recvfromclient(client):
    while True:
        # 接收数据包前八个字节。来解析出类型和大小
        datapack = client.recv(8)
        if len(datapack) == 0:
            raise Exception("客户端断开连接")
        Type, size = struct.unpack("ii", datapack)
        temp = client.recv(1024)
        datax = base64.b64decode(temp).decode('gb2312')
        data = datax.split('\n')
        return Type, data

class Socket:
    def __init__(self):
        # 创建套接字
        self.Socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # 绑定端口
        self.Socket.bind(("127.0.0.1", 6666))
        # 监听端口
        self.Socket.listen(socket.SOMAXCONN)
        # 创建数据库和表,如果存在了则不再创建
        global _db  # 创建一个全局对象
        _db = QQSQL.Sql()
        _db.createDBTable()

    # 从客户端接收信息
    def recvclientData(self, handler):
        Type, args = None, None
        while True:
            try:
                # 接收数据
                Type, args = recvfromclient(handler.socket)
            except Exception as e:
                del _clientDict[handler.user_id]
                break
            try:
                Type = DataType(Type)
            except:
                sendtoclientsocket(handler.socket, DataType.ERR, "无效的请求")
            # 获取函数
            func = getattr(handler, Type.name)
            handler.args = args
            func()

    # 运行
    def run(self):
        # 服务器连接并创建线程接收信息
        while True:
            client, addr = self.Socket.accept()
            h = Handler()
            h.socket = client
            h.addr = addr
            print('接入新客户端: ', addr)
            thread = threading.Thread(target=self.recvclientData, args=(h,)).start()


_clientDict = {}
_FriendDict = []
_GroupDict = []
_GroupMember = []
lock = threading.Lock()

if __name__ == '__main__':
    socket = Socket()
    socket.run()
