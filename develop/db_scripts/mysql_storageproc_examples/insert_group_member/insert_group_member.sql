
DELIMITER //
DROP PROCEDURE IF EXISTS insert_group_member_by_groupid;
CREATE PROCEDURE insert_group_member_by_groupid( IN in_groupid int, IN in_insert_count int, IN in_start_userid int )
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE start_userid INT DEFAULT 0;
    DECLARE res_userid INT DEFAULT 0;
    DECLARE time_now INT DEFAULT 0;
           
    DECLARE cur1 CURSOR FOR select userid from tm_users where userid > in_start_userid and username<>'0' and ( userid not in ( select groupmemberid from tm_group_member where groupid=in_groupid ) ) limit in_insert_count;
     
     
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done=TRUE;
    OPEN cur1;
    
    START TRANSACTION;
        select unix_timestamp(now()) into time_now;
        
        select userid,username,email from tm_users where userid > in_start_userid and username<>'0' and ( userid not in ( select groupmemberid from tm_group_member where groupid=in_groupid ) ) limit in_insert_count;
    
        read_loop: LOOP
            FETCH cur1 INTO res_userid;
            IF done THEN 
                LEAVE read_loop;
            END IF;
            
            insert into tm_group_member(groupid,groupmemberid,groupmembertype,groupmemberactivetime,modifytime) values(in_groupid,res_userid,'normal',time_now,time_now);           
        END LOOP;
    COMMIT;
END
//


DELIMITER //
DROP PROCEDURE IF EXISTS insert_group_member_by_groupnum;
CREATE PROCEDURE insert_group_member_by_groupnum( IN in_groupnum int, IN in_insert_count int, IN in_start_userid int )
BEGIN
    DECLARE grpid INT DEFAULT 0;
    
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    START TRANSACTION;
        select groupid into grpid from tm_group where groupnum =in_groupnum;
        call insert_group_member_by_groupid( grpid, in_insert_count, in_start_userid );
    COMMIT;
END
//

