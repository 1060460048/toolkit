
/*
* 根据需要把用户头像原来为0(默认图标)设置成1-47中的任意一个。
*
*  功能：userface=0 依次设置成 1-47
*
*  调用：call update_tm_userface( 0, 1, 47 );
**/

DELIMITER //
DROP PROCEDURE IF EXISTS update_tm_userface;
CREATE PROCEDURE update_tm_userface( IN in_from_userface int, IN in_to_userface_begin int, IN in_to_userface_end int )
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE time_now INT DEFAULT 0;    
    DECLARE res_userid INT DEFAULT 0;   
    DECLARE new_userface INT DEFAULT in_from_userface;           
    DECLARE total_update INT DEFAULT 0;         
    DECLARE total_update_info varchar(128);   
    DECLARE cur1 CURSOR FOR select userid from tm_users where userface=0; 
     
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done=TRUE;
    OPEN cur1;
    
    START TRANSACTION;
        #输出需要修改的总数
        select concat('Going to update ', count(*)) into total_update_info from tm_users where userface=0;
        select total_update_info;
        
        select unix_timestamp(now()) into time_now;
    
        read_loop: LOOP
            FETCH cur1 INTO res_userid;
            IF done THEN 
                LEAVE read_loop;
            END IF;
            
            #达到最后一个头像则从第一个开始。
            IF new_userface = in_from_userface OR new_userface > in_to_userface_end THEN
                set new_userface =in_to_userface_begin;
            END IF;
            
            # 修改头像及时间
            update tm_users set userface =new_userface ,modifytime=time_now where userid =  res_userid;
            set new_userface = new_userface+1;
            
            # 输出当前修改的信息
            SELECT userid,username,userface,new_userface,total_update from tm_users where userid =  res_userid;
            set total_update = total_update+1;
        END LOOP;
    COMMIT;
END
//

