# 互联网应用开发技术

## 后端开发

### 简单的三层架构（DAO + Service + Controller）：

DAO 用于与数据库的直接交互，定义增删改查等操作

Service 负责业务逻辑，跟功能相关的代码一般写在这里，编写、调用各种方法对 DAO 取得的数据进行操作

Controller 负责数据交互，即接收前端发送的数据，通过调用 Service 获得处理后的数据并返回

### ORM框架

ORM（Object Relational Mapping）框架采用元数据来描述对象与关系映射的细节，元数据一般采用XML格式，并且存放在专门的对象一映射文件中。

这个项目用的是JPA.(JPA是一套规范,主要是为了简化现有的持久化开发工作和整合ORM技术)

Spring Data JPA 是 Spring 基于 ORM 框架（hibernate）、JPA 规范的基础上封装的一套JPA应用框架，可使开发者用极简的代码即可实现对数据的访问和操作(不用写MySQL语句了哈哈哈哈哈哈哈)。
它提供了包括增删改查等在内的常用功能，且易于扩展。

![Spring Data JPA 提供的编程接口继承关系图](https://img2018.cnblogs.com/blog/1091033/201904/1091033-20190423200533888-1022949411.png) 
