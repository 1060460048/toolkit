
/*
* 清空tm_domain_member表,根据tm_users表插入对应的记录到tm_domain_member。
**/


show create procedure focustm.oa_tm_zlj
CREATE DEFINER=`root`@`localhost` PROCEDURE `oa_tm_zlj`()
COMMENT '此存储过程是替换陈志禹编写的oa_tm存储过程，因为存储过程中的判断条件不合适'
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE loop_userid INT DEFAULT 0;
    DECLARE loop_username varchar(255);
    DECLARE loop_domaindid INT DEFAULT 3;
    DECLARE loop_email varchar(255);
    DECLARE total_update INT DEFAULT 0; 
    DECLARE curl CURSOR FOR select userid, username, email, domainid from tm_users where userid not in (select userid from tm_domain_member);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    
    BEGIN
        ROLLBACK;    
        RESIGNAL;
    END;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done=TRUE;
    OPEN curl;

    insert into
    tm_users(userid,username,nickname,idcard,active_flag,telephone,domainid,title,email,domainuserid)
    select id,ldap_username,replace(name,'',''),employee_no,active_flag,telephone,3,duty,email,concat('focustm=',id) from employee where id not in
    (select userid from tm_users);
    
    insert into tm_department(departmentid,departmentname,parentid,order_no)
    select dept_id,dept_name,(up_dept+0),order_no from dept where dept_id
    not in (select departmentid from tm_department);

    read_loop: LOOP
        FETCH curl INTO loop_userid,loop_username,loop_email, loop_domaindid;
        IF done THEN 
            LEAVE read_loop;
        END IF;

        insert into tm_domain_member(domainid, userid, domainuserid, domainusername, domainemail) 
        values(loop_domaindid, loop_userid,CAST(loop_userid as char(100)),loop_username,loop_email);

        set total_update = total_update+1;
    END LOOP;

    commit;
END
 
 
show create procedure focustm.oa_tm_update
CREATE DEFINER=`root`@`localhost` PROCEDURE `oa_tm_update`()
BEGIN
    update tm_users a,employee b set
    a.username=b.ldap_username,a.nickname=replace(b.name,'',''),a.idcard=b.employee_no,a.telephone=b.telephone,
    a.active_flag=b.active_flag,a.title=b.duty,a.email=b.email where a.userid=b.id;

    update tm_department_member a,employee b set a.departmentid=b.dept_id
    where a.userid=b.id;

    update tm_department a,dept b set
    a.departmentname=b.dept_name,a.parentid=(b.up_dept+0),a.order_no=b.order_no
    where a.departmentid=b.dept_id;

    update tm_domain_member a, tm_users b set 
    a.domainid=b.domainid,a.domainuserid=CAST(b.userid as char(100)),a.domainusername=b.username,a.domainemail=b.email
    where a.userid=b.userid;

    commit;
END
