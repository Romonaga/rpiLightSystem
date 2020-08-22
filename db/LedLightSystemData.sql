-- MySQL dump 10.13  Distrib 8.0.21, for Linux (x86_64)
--
-- Host: 127.0.0.1    Database: LedLightSystem
-- ------------------------------------------------------
-- Server version	8.0.21-0ubuntu0.20.04.4

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
-- Dumping data for table `lDirection`
--

LOCK TABLES `lDirection` WRITE;
/*!40000 ALTER TABLE `lDirection` DISABLE KEYS */;
INSERT INTO `lDirection` VALUES (1,'Up'),(2,'Right'),(3,'Down'),(4,'Left');
/*!40000 ALTER TABLE `lDirection` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `lLightSystemFeatures`
--

LOCK TABLES `lLightSystemFeatures` WRITE;
/*!40000 ALTER TABLE `lLightSystemFeatures` DISABLE KEYS */;
INSERT INTO `lLightSystemFeatures` VALUES (1,'Lights By Motion'),(2,'Lights By Light'),(3,'Lights by Time'),(4,'Lights By Lux');
/*!40000 ALTER TABLE `lLightSystemFeatures` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `lMatrixDirection`
--

LOCK TABLES `lMatrixDirection` WRITE;
/*!40000 ALTER TABLE `lMatrixDirection` DISABLE KEYS */;
INSERT INTO `lMatrixDirection` VALUES (1,'Top Left to Right'),(2,'Top Right Down'),(3,'Bottom Right Up'),(4,'Top Right to Left'),(5,'Matrix Top Down'),(6,'Matrix Top Down Right');
/*!40000 ALTER TABLE `lMatrixDirection` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `lMatrixShapes`
--

LOCK TABLES `lMatrixShapes` WRITE;
/*!40000 ALTER TABLE `lMatrixShapes` DISABLE KEYS */;
INSERT INTO `lMatrixShapes` VALUES (1,'Circle'),(2,'Square'),(3,'Rectangle'),(4,'Triangle');
/*!40000 ALTER TABLE `lMatrixShapes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `lMatrixTypes`
--

LOCK TABLES `lMatrixTypes` WRITE;
/*!40000 ALTER TABLE `lMatrixTypes` DISABLE KEYS */;
INSERT INTO `lMatrixTypes` VALUES (0,'None'),(1,'Matrix Art'),(2,'Matrix Text'),(3,'Matrix Show'),(4,'Matrix Shape'),(5,'Matrix Display File');
/*!40000 ALTER TABLE `lMatrixTypes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `lPosition`
--

LOCK TABLES `lPosition` WRITE;
/*!40000 ALTER TABLE `lPosition` DISABLE KEYS */;
INSERT INTO `lPosition` VALUES (1,'Top'),(2,'Center'),(3,'Bottom');
/*!40000 ALTER TABLE `lPosition` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `lStripType`
--

LOCK TABLES `lStripType` WRITE;
/*!40000 ALTER TABLE `lStripType` DISABLE KEYS */;
INSERT INTO `lStripType` VALUES (1,'NEO_WS2811_STRIP_RGB'),(2,'NEO_WS2811_STRIP_RBG'),(3,'NEO_WS2811_STRIP_GRB'),(4,'NEO_WS2811_STRIP_GBR'),(5,'NEO_WS2811_STRIP_BRG'),(6,'NEO_WS2811_STRIP_BGR'),(7,'NEO_SK6812_STRIP_RGBW'),(8,'NEO_SK6812_STRIP_GRBW'),(9,'2121_MATRIX Boards');
/*!40000 ALTER TABLE `lStripType` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `lightShows`
--

LOCK TABLES `lightShows` WRITE;
/*!40000 ALTER TABLE `lightShows` DISABLE KEYS */;
INSERT INTO `lightShows` VALUES (1,'Blink',1,0,1,0,1,0,18,0,0),(2,'Chaser',1,1,1,0,1,0,13,0,0),(3,'Theater Chase',1,0,1,0,1,0,2,0,0),(4,'Theater Chase Rainbow',1,0,1,0,1,0,3,0,0),(5,'Color 3 Reverse',1,3,1,0,1,0,14,0,0),(6,'Cylon',1,1,1,1,1,0,15,0,0),(7,'Color Wipe',1,1,1,0,1,0,16,0,0),(8,'Half n Half',1,2,0,0,0,0,9,0,0),(9,'Rainbow',1,0,1,0,1,0,7,0,0),(10,'Rainbow Cycle',1,0,1,0,1,0,4,0,0),(11,'Neorand',1,0,0,0,1,0,5,0,0),(12,'Flame',1,1,1,0,1,0,8,0,0),(13,'Color 1/3 ',1,3,0,0,0,0,10,0,0),(14,'Color 1/4',1,4,0,0,0,0,11,0,0),(15,'Tri-Color Chaser',1,3,1,0,1,0,6,0,0),(16,'Display Color',1,1,0,0,0,0,1,0,0),(17,'Color Every',1,1,0,0,0,1,12,0,0),(18,'Twinkle Overlay',1,2,1,0,1,0,17,0,0),(19,'Pulse Overlay',1,0,1,0,1,0,19,0,0),(20,'Scanner',1,1,1,1,1,0,20,0,0),(21,'Bouncing Balls',1,1,1,0,1,0,127,0,0),(22,'Fade',0,0,1,1,1,0,31,0,0),(23,'Matrix Art',1,0,0,0,0,0,32,1,0),(24,'Matrix Text',1,1,1,0,1,0,32,2,0),(25,'Delay Show',1,0,1,0,0,0,33,0,0),(26,'Matrix Trip',1,1,1,0,1,0,32,3,0),(27,'Circle',1,1,0,0,0,0,40,4,1),(28,'Square',1,1,0,0,0,0,40,4,2),(29,'Triangle',1,1,0,0,0,0,40,4,4),(30,'Art to JPG',1,0,0,0,0,0,40,0,0),(31,'Display File JPG/PNG/JIF',1,0,1,0,0,0,40,5,0),(32,'Matrix Vol Bars',1,4,1,1,1,0,32,0,0),(33,'Matrix Scroll Time',1,1,1,0,1,0,32,2,0),(34,'Matrix Ant Art',1,4,1,0,0,0,32,3,0),(35,'Matrix Kaleidoscope',1,4,1,0,1,0,32,3,0),(36,'Matrix Color Evolution',1,0,1,0,1,0,32,3,0);
/*!40000 ALTER TABLE `lightShows` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-08-22 17:52:59
