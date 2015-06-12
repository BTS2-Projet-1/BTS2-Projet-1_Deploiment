DELETE FROM "Console";
CREATE TABLE IF NOT EXISTS "Console" 
  ("idConsole" VARCHAR PRIMARY KEY  NOT NULL  UNIQUE ,
  "LinkedBoxes" INTEGER NOT NULL );

DELETE FROM "DCB_Controler";
CREATE TABLE IF NOT EXISTS "DCB_Controler" 
  ("idDCB_Controler" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL 
  , "Linked_idDoorLocker_1" INTEGER NOT NULL , 
  "Linked_idDoorLocker_2" INTEGER NOT NULL );

DELETE FROM "DoorLocker";
CREATE TABLE IF NOT EXISTS "DoorLocker" 
  ("idDoorLocker" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
  "Linked_idDCB_Controler" INTEGER NOT NULL , 
  "Linked_idDoor" INTEGER, 
  "DoorLocker_OpenState" BOOL NOT NULL , 
  "DoorLocker_LockState" BOOL NOT NULL );

DELETE FROM "Box";
CREATE TABLE IF NOT EXISTS "Box" 
  ("idBox" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
  "BoxTypeSize" VARCHAR NOT NULL  DEFAULT A, 
  "BoxIsUsed" BOOL NOT NULL  DEFAULT false, 
  "BoxIsDammaged" BOOL NOT NULL  DEFAULT false,
  "Linked_idDoorLocker_1" INTEGER NOT NULL , 
  "Linked_idDoorLocker_2" INTEGER NOT NULL , 
  "Pos_X" INTEGER NOT NULL , 
  "Pos_Y" INTEGER NOT NULL );

DELETE FROM "ExtractCode";
CREATE TABLE IF NOT EXISTS "ExtractCode" 
  ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
  "idExtractCode" VARCHAR NOT NULL , 
  "Linked_idPackage" VARCHAR NOT NULL , 
  "ExtractCodeExpiryTime" DATETIME NOT NULL );

DELETE FROM "Package";
CREATE TABLE IF NOT EXISTS "Package" 
  ("idPackage" VARCHAR PRIMARY KEY  NOT NULL , 
  "Linked_idExtractCode" VARCHAR NOT NULL , 
  "Linked_idBox" INTEGER NOT NULL );

DELETE FROM "User";
CREATE TABLE IF NOT EXISTS "User" 
  ("UserType" INTEGER NOT NULL , 
  "UserCompanyPass" VARCHAR NOT NULL , 
  "UserPersonalPass" VARCHAR NOT NULL , 
  "UserPassWord" VARCHAR NOT NULL , PRIMARY KEY ("UserCompanyPass", "UserPersonalPass", "UserPassWord"));

DELETE FROM "PerformedActions";
CREATE TABLE IF NOT EXISTS "PerformedActions" 
  ("idPerformedAction" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
  "EditedTable" VARCHAR NOT NULL , 
  "ActionMessage" VARCHAR NOT NULL , 
  "Time" DATETIME NOT NULL );
