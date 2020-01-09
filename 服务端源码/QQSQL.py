import pymysql



class Sql(object):
    # 构造函数，主要用于连接指定的数据库
    def __init__(self):
        try:
            # 获取一个连接对象，获取的时候可能会产生异常
            self.connect = pymysql.connect(host="127.0.0.1", port=3306, user="root", password="11111111A!root",
                                           cursorclass=pymysql.cursors.DictCursor)
            # 创建指针
            self.cursor = self.connect.cursor(cursor=pymysql.cursors.DictCursor)
        except Exception as e:
            print(e)

    def createDBTable(self):
        # 创建数据库
        self.cursor.execute("CREATE DATABASE IF NOT EXISTS MYQQ")
        # 连接MYQQ数据库
        self.cursor.execute("use MYQQ")
        # 创建一个名为user_info的表
        sql = """CREATE TABLE IF NOT EXISTS `user_info` (
                  `user_id` int(16) not null primary key,
                  `user_pswd` varchar(32) not null,
                  `isOnline` tinyint(1) not null
                  )"""
        self.cursor.execute(sql)
        # 创建一个好友表
        sql = """CREATE TABLE IF NOT EXISTS `friendship_info` (
                          `id` int auto_increment primary key,
                          `user_id1` int(16) not null ,
                          `user_id2` int(16) not null 
                          )"""
        self.cursor.execute(sql)
        # 创建一个聊天记录表
        sql = """CREATE TABLE IF NOT EXISTS `chat_record` (
                                  `id` int auto_increment primary key,
                                  `user_id1` int(16) not null ,
                                  `user_id2` int(16) not null, 
                                  `record` varchar(1024) not null
                                  )"""
        self.cursor.execute(sql)
        # 创建一个聊天室表
        sql = """CREATE TABLE IF NOT EXISTS `room_info` (
                                          `room_id` int(16) not null primary key,
                                          `boss` int(16) not null 
                                          )"""
        self.cursor.execute(sql)
        # 创建一个聊天室群员表
        sql = """CREATE TABLE IF NOT EXISTS `room_Member` (
                                          `room_id` int(16) not null ,
                                          `user_id` int(16) not null
                                          )"""
        self.cursor.execute(sql)
        # 创建一个聊天室记录表
        sql = """CREATE TABLE IF NOT EXISTS `room_record` (
                                          `room_id` int(16) not null,
                                          `user_id` int(16) not null, 
                                          `record` varchar(1024) not null
                                          )"""
        self.cursor.execute(sql)

if __name__ == '__main__':
    sql = Sql()