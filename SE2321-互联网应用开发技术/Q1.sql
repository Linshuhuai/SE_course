/* Q1 */
select name
from instructor
where dept_name='Biology';

/* Q2 */
select title
from course
where dept_name='Comp. Sci.' and credits=3;

/* Q3 */
select takes.ID,course.course_id,course.title
from course,takes
where takes.ID='12345' and takes.course_id=course.course_id;

/* Q4 */
select takes.ID,sum(course.credits)
from course, takes
where takes.course_id=course.course_id and takes.ID='12345'
group by takes.ID;

/* Q5 */
select ID,tot_cred
from student;

/* Q6 */
select distinct name
from course,takes,student
where takes.ID=student.ID and takes.course_id=course.course_id and course.dept_name='Comp. Sci.';

/* Q7 */
select distinct instructor.ID
from instructor,teaches
where not exists
(
select *
from teaches
where instructor.ID=ID
);

/* Q8 */
select distinct instructor.ID, instructor.name
from instructor,teaches
where not exists
(
select *
from teaches
where instructor.ID=ID
)