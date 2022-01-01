-- Q1
create table actors (
    AID varchar(20),
    name varchar(50),
    primary key (AID)
);
create table movies (
    MID varchar(20),
    title varchar(50),
    primary key (MID)
);
create table actor_role (
    MID varchar(20),
    AID varchar(20),
    rolename(50),
    primary key (MID, AID, rolename),
    foreign key (MID) references movies(MID),
    foreign key (AID) references actors(AID)
);

-- Q2
delete from actor_role;
delete from actors;
delete from movies;
insert into actors values ('01','Charlie Chaplin');
insert into movies values ('M1','City Lights');
insert into actor_role values ('M1','01','Tramp');
insert into actors values ('02','Stephen Chow');
insert into movies values ('M2','Kung Fu Hustle');
insert into actor_role values ('M2','02','Kung Fu Hustle');
insert into actors values ('03','Jay Chou');
insert into movies values ('M3','Initial D');
insert into actor_role values ('M3','03','Fujiwara Takumi');
insert into actors values ('04','Shuhuai Lin');

-- Q3
select name, title, count(rolename)
from actor_role natural join movies natural join actors
where name = 'Charlie Chaplin' group by MID;

-- Q4
select name from actors
where name not in (
   select distinct name
   from actor_role natural join actors
);

-- Q5
select * from (
(select name, title
  from actors, movies, actor_role
  where actors.AID = actor_role.AID and movies.MID = actor_role.MID)
 union
(select name as name, null as title from
(select name from actors
 where name not in
(select distinct name
 from actor_role natural join actors)
) as alias1)
) as alias2;


