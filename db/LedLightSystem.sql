CREATE DATABASE  IF NOT EXISTS `LedLightSystem` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `LedLightSystem`;
-- MySQL dump 10.13  Distrib 8.0.20, for Linux (x86_64)
--
-- Host: 127.0.0.1    Database: LedLightSystem
-- ------------------------------------------------------
-- Server version	8.0.20-0ubuntu0.20.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `lLightSystemFeatures`
--

DROP TABLE IF EXISTS `lLightSystemFeatures`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lLightSystemFeatures` (
  `ID` int NOT NULL,
  `featureName` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lStripType`
--

DROP TABLE IF EXISTS `lStripType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lStripType` (
  `ID` int NOT NULL,
  `stripName` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lightShows`
--

DROP TABLE IF EXISTS `lightShows`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lightShows` (
  `ID` int NOT NULL,
  `showName` varchar(100) DEFAULT NULL,
  `enabled` int DEFAULT NULL,
  `numColors` int DEFAULT NULL,
  `hasDelay` tinyint DEFAULT NULL,
  `hasWidth` tinyint DEFAULT NULL,
  `hasMinutes` tinyint DEFAULT '1',
  `colorEvery` tinyint DEFAULT '0',
  `showOrder` tinyint DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lightSystemFeatures`
--

DROP TABLE IF EXISTS `lightSystemFeatures`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lightSystemFeatures` (
  `featureId` int NOT NULL,
  `lightSystemId` int NOT NULL,
  `featureGpio` int DEFAULT '0',
  `featurePlayList` int DEFAULT '0',
  `motionDelayOff` int DEFAULT '0',
  `timeFeatureStart` varchar(45) DEFAULT '0',
  `timeFeatureEnd` varchar(45) DEFAULT '0',
  `luxThreshHold` int DEFAULT '0',
  PRIMARY KEY (`featureId`,`lightSystemId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lightSystems`
--

DROP TABLE IF EXISTS `lightSystems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lightSystems` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `systemName` varchar(100) DEFAULT NULL,
  `stripType` int DEFAULT NULL,
  `stripHeight` int DEFAULT NULL,
  `stripWidth` int DEFAULT NULL,
  `dma` int DEFAULT NULL,
  `gpio` int DEFAULT NULL,
  `serverHostName` varchar(50) DEFAULT NULL,
  `brightness` int DEFAULT NULL,
  `enabled` int DEFAULT NULL,
  `userId` int DEFAULT NULL,
  `gamma` double DEFAULT NULL,
  `twitchSupport` tinyint DEFAULT '0',
  `mqttRetries` int DEFAULT '2000',
  `mqttRetryDelay` int DEFAULT '2500',
  `twitchMqttQueue` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=82 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lumaUsers`
--

DROP TABLE IF EXISTS `lumaUsers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lumaUsers` (
  `username` varchar(50) NOT NULL,
  `ID` int NOT NULL AUTO_INCREMENT,
  `password` varchar(50) DEFAULT NULL,
  `email` varchar(100) DEFAULT NULL,
  `phonenumber` varchar(25) DEFAULT NULL,
  `twitter` varchar(50) DEFAULT NULL,
  `authorized` tinyint DEFAULT '0',
  `isAdmin` tinyint DEFAULT '0',
  PRIMARY KEY (`ID`,`username`),
  UNIQUE KEY `username_UNIQUE` (`username`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `showsRan`
--

DROP TABLE IF EXISTS `showsRan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `showsRan` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `systemId` int DEFAULT NULL,
  `userID` int DEFAULT NULL,
  `showId` int DEFAULT NULL,
  `showParms` json DEFAULT NULL,
  `timeStamp` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=601 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `twitchChannels`
--

DROP TABLE IF EXISTS `twitchChannels`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `twitchChannels` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `channel` varchar(45) DEFAULT NULL,
  `mqttQueue` varchar(45) DEFAULT NULL,
  `enabled` tinyint DEFAULT NULL,
  `allowAllTwitchUsers` tinyint DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `twitchUsers`
--

DROP TABLE IF EXISTS `twitchUsers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `twitchUsers` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `twitchUser` varchar(45) DEFAULT NULL,
  `enabled` tinyint DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `userPlaylist`
--

DROP TABLE IF EXISTS `userPlaylist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `userPlaylist` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `userID` int DEFAULT NULL,
  `playlistName` varchar(50) DEFAULT NULL,
  `showParms` json DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=72 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-07-19 14:15:38
