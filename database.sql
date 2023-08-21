CREATE DATABASE db_arduino_test CHARACTER SET = 'utf8' COLLATE = 'utf8_general_ci';

USE db_arduino_test;

CREATE TABLE relations (
	relation_id INT AUTO_INCREMENT PRIMARY KEY,
	relation_category char(1) NOT NULL,
	description varchar(50) NOT NULL
);

CREATE TABLE users (
	user_id INT AUTO_INCREMENT PRIMARY KEY,
	user_firstname VARCHAR(64) NOT NULL,
	user_lastname VARCHAR(64) NOT NULL,
	user_rfid_uid VARCHAR(8) NOT NULL,
	user_pin VARCHAR(5) NOT NULL,
	relation_id INT NOT NULL,
	active BOOLEAN NOT NULL,
	FOREIGN KEY (relation_id) REFERENCES relations(relation_id)
);

CREATE TABLE alerts (
	alert_id INT AUTO_INCREMENT PRIMARY KEY,
	user_id INT NOT NULL,
	datetime_created DATETIME NOT NULL,
	datetime_confirmed DATETIME,
	confirmed BOOLEAN NOT NULL,
	FOREIGN KEY (user_id) REFERENCES users(user_id)
);

insert into relations(relation_category, description) values ("A", "Son");
insert into relations(relation_category, description) values ("A", "Daughter");
insert into relations(relation_category, description) values ("A", "Grandson");
insert into relations(relation_category, description) values ("A", "Granddaughter");
insert into relations(relation_category, description) values ("A", "Niece");
insert into relations(relation_category, description) values ("A", "Nephew");
insert into relations(relation_category, description) values ("B", "Evening carer");
insert into relations(relation_category, description) values ("B", "Morning carer");
insert into relations(relation_category, description) values ("C", "School friend");
insert into relations(relation_category, description) values ("C", "Work friend");
insert into relations(relation_category, description) values ("D", "Nextdoor neighbour");
insert into relations(relation_category, description) values ("D", "Across the road neighbour");

insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (1, 'Merry', 'Kinchley', '6D 90 12 33', 'D8719', 12, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (2, 'Matthias', 'Farrens', 'FF 98 C4 34', 'A6515', 3, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (3, 'Kendra', 'Kares', '8C 99 2A A3', 'A4169', 5, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (4, 'Kellby', 'Guilford', '0A 11 51 E6', 'D1997', 11, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (5, 'Andriette', 'Antat', '99 AB B5 3A', 'A0335', 1, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (6, 'Ferdinand', 'Armitt', 'A0 11 87 48', 'A9702', 5, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (7, 'Brenn', 'Roxbrough', 'D7 E7 00 4C', 'A6605', 4, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (8, 'Maurice', 'Charge', 'A6 6C 87 36', 'B6311', 7, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (9, 'Callie', 'Archbold', 'F7 F5 22 77', 'C2181', 9, true);
insert into users (user_id, user_firstname, user_lastname, user_rfid_uid, user_pin, relation_id, active) values (10, 'Theresa', 'Semon', '09 94 F8 9A', 'D8514', 12, true);

insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (1, 8, '2023-04-12 13:41:42', '', false);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (2, 1, '2023-04-17 02:56:37', '2023-04-17 03:23:44', true);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (3, 7, '2023-01-08 09:36:12', '', false);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (4, 7, '2023-05-02 02:20:57', '', false);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (5, 3, '2023-03-04 09:51:19', '2023-03-04 09:53:57', true);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (6, 8, '2023-04-24 09:56:02', '', false);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (7, 9, '2023-01-04 12:52:08', '2023-01-04 13:07:11', true);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (8, 10, '2023-04-26 19:43:33', '', false);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (9, 6, '2023-02-19 22:16:01', '2023-02-19 22:20:36', true);
insert into alerts (alert_id, user_id, datetime_created, datetime_confirmed, confirmed) values (10, 10, '2023-04-02 19:28:00', '2023-04-02 19:36:45', true);
