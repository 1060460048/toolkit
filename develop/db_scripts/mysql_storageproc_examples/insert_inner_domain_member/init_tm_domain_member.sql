
/*
* 清空tm_domain_member表,根据tm_users表插入对应的记录到tm_domain_member。
**/

DELIMITER 
DROP PROCEDURE IF EXISTS insert_domain_member;
CREATE PROCEDURE insert_domain_member()
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE time_now INT DEFAULT 0;    
    DECLARE loop_userid INT DEFAULT 0;  
    DECLARE loop_username varchar(255);  
	DECLARE loop_email varchar(255);          
    DECLARE total_update INT DEFAULT 0;     
    DECLARE r_domainid INT DEFAULT 0;      
    DECLARE cur1 CURSOR FOR select userid,username,email from tm_users;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done=TRUE;
    OPEN cur1;
    
    #输出需要修改的总数
    select count(*) as "Total update" from tm_users;

    select domainid into r_domainid from tm_domain where domain='focustm';
    select unix_timestamp(now()) into time_now;
	truncate tm_domain_member;

    read_loop: LOOP
        FETCH cur1 INTO loop_userid,loop_username,loop_email;
        IF done THEN 
            LEAVE read_loop;
        END IF;

        select userid,username,total_update,"insert" from tm_users where userid = loop_userid;
        insert into tm_domain_member(domainid,userid,domainuserid,domainusername,domainemail) 
        values(r_domainid,loop_userid,CAST(loop_userid as char(100)),loop_username,loop_email);

        set total_update = total_update+1;
    END LOOP;
END

