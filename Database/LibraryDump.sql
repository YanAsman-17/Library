-- --------------------------------------------------------
-- Хост:                         127.0.0.1
-- Версия сервера:               8.0.32 - MySQL Community Server - GPL
-- Операционная система:         Win64
-- HeidiSQL Версия:              12.5.0.6677
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Дамп структуры для таблица diglib.authors
CREATE TABLE IF NOT EXISTS `authors` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `first_name` varchar(30) NOT NULL DEFAULT '',
  `last_name` varchar(30) NOT NULL DEFAULT '',
  `surname` varchar(30) NOT NULL DEFAULT '',
  `counrty` varchar(30) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  FULLTEXT KEY `fti_authors_fio` (`first_name`,`last_name`,`surname`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Дамп данных таблицы diglib.authors: ~13 rows (приблизительно)
INSERT INTO `authors` (`id`, `first_name`, `last_name`, `surname`, `counrty`) VALUES
	(1, 'Александр', 'Пушкин', 'Сергеевич', 'Россия'),
	(2, 'Лев', 'Толстой', 'Николаевич', 'Россия'),
	(3, 'Федор', 'Достоевский', 'Михайлович', 'Россия'),
	(4, 'Марк', 'Твен', '', 'США'),
	(5, 'Джордж', 'Оруэлл', '', 'Великобритания'),
	(6, 'Михаил', 'Булгаков', 'Афанасьевич', 'Россия'),
	(7, 'Илья', 'Ильф', 'Арнольдович', 'СССР'),
	(8, 'Евгений', 'Петров', 'Петрович', 'СССР'),
	(9, 'Антон', 'Чехов', 'Павлович', 'Россия'),
	(10, 'Оскар', 'Уайльд', '', 'Великобритания'),
	(11, 'Джейн', 'Остин', '', 'Великобритания'),
	(12, 'Эрнест', 'Хемингуэй', '', 'США'),
	(13, 'Фрэнсис', 'Фицджеральд', '', 'США');

-- Дамп структуры для событие diglib.autoReturnBooksAfterIssueTime
DELIMITER //
CREATE EVENT `autoReturnBooksAfterIssueTime` ON SCHEDULE EVERY 24 HOUR STARTS '2023-11-09 00:00:00' ON COMPLETION NOT PRESERVE ENABLE DO CALL returnBooksAfterIssueTime()//
DELIMITER ;

-- Дамп структуры для событие diglib.autoReturnBooksAfterReservedTime
DELIMITER //
CREATE EVENT `autoReturnBooksAfterReservedTime` ON SCHEDULE EVERY 1 DAY STARTS '2023-12-23 04:18:02' ON COMPLETION NOT PRESERVE ENABLE DO BEGIN
CALL returnBooksAfterReservedTime();
END//
DELIMITER ;

-- Дамп структуры для таблица diglib.books
CREATE TABLE IF NOT EXISTS `books` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL DEFAULT '',
  `full_descript` mediumtext,
  `year_publication` mediumint NOT NULL,
  `image` varchar(100) NOT NULL DEFAULT '',
  `days` tinyint DEFAULT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `fti_books_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Дамп данных таблицы diglib.books: ~14 rows (приблизительно)
INSERT INTO `books` (`id`, `name`, `full_descript`, `year_publication`, `image`, `days`) VALUES
	(1, 'Евгений Онегин', 'В своем поэтическом романе Пушкин переносит нас в Россию начала XIX века, где герой Евгений Онегин вступает в противоречие с общепринятыми конвенциями высшего общества. Чередуя стихотворные главы, он рассказывает историю Онегина и его сложных отношений с прекрасной Татьяной Лариной и дружбе с блистательным графом Ленским. "Евгений Онегин" поражает своей красотой, глубиной и смыслом, призывая к поиску счастья и истинного смысла жизни.', 1833, 'resources/imgBooks/Onegin.jpg', 21),
	(2, 'Война и мир. Том 1', 'Величие истории в сочетании с судьбами обыкновенных людей - вот основа этой знаменитой романной эпопеи. Изображая события во время войн Наполеона, Толстой объединяет масштабные сражения и внутренние монологи героев, рассказывая о любви, верности, предательстве и стремлении к смыслу жизни. "Война и мир" исследует не только войну, но и ее влияние на людей и их судьбы.', 1869, 'resources/imgBooks/WarAndPiece.jpg', 45),
	(3, 'Преступление и наказание', 'В своем великом романе Достоевский исследует нравственные дилеммы и психологическую жизнь главного героя, студента Раскольникова, который совершает ужасное преступление. Рушащиеся морали, душевный конфликт и внутренняя борьба с собой - все это выходит наружу в ходе жестких следствий и наказания. "Преступление и наказание" является глубоким обращением к нравственным вопросам и ценности каждой жизни.', 1866, 'resources/imgBooks/Преступление и наказание.jpg', 30),
	(4, 'Приключения Тома Сойера', 'Захватывающий роман, который рисует портрет неутомимого мальчика, готового на все ради приключений. Веселые и опасные похождения Тома, его верных друзей и любопытную любовь к девочке Бекки не оставят равнодушным ни одного читателя. Старинный американский городок, живые образы героев и теплая атмосфера детства - все это строится вокруг Тома Сойера. Игры, секреты, приятные и опасные приключения - готовьтесь встретиться с настоящими хулиганами!', 1876, 'resources/imgBooks/Том Сойер.jpg', 30),
	(5, '1984', 'В мрачном мире будущего, где правит беспощадная тоталитарная система, Винстон Смит начинает сомневаться в истинности своей реальности. Однако его стремление к свободе, правде и человеческому достоинству вступает в противоречие с жестким контролем Большого Брата. Роман "1984" ошеломляет своей прозрительностью и предупреждает о возможных последствиях потери свободы мысли и индивидуальности.', 1949, 'resources/imgBooks/1984.jpeg', 21),
	(6, 'Мастер и Маргарита', 'В самое сердце сталинской Москвы следует сатана в компании своих странных пришельцев. Это роман об искусстве, свободе и отношении общества к силам зла и добра. В мире, где мифы и реальность переплетаются, Мастер и его возлюбленная Маргарита вступают в сделку с обезображенным прокурором и оживляют страсти в Москве 1930-х годов. "Мастер и Маргарита" навсегда останется шедевром русской литературы и мудрым исследованием сущности человеческой души.', 1967, 'resources/imgBooks/Мастер и маргарита.jpg', 30),
	(7, 'Двенадцать стульев', 'В поисках бесценных бриллиантов, спрятанных в одном из двенадцати стульев, Остап Бендер и его неуклюжий спутник Ипполит Матвеевич путешествуют по Советскому Союзу. Полный смеха и абсурда, этот классический сатирический роман осмеивает охоту за материальным богатством и героический культивированный образ советской действительности.', 1928, 'resources/imgBooks/12.jpg', 30),
	(8, 'Золотой теленок', 'Путешествие героев Остапа Бендера и Кисы Воробьянинова в поисках наследства Золотого теленка разворачивается по полномасштабным комедийным сюжетам. В политической мозаике Советского Союза, Ильф и Петров разыгрывают культурные нормы и правила для достижения печатного успеха. "Золотой теленок" - забавный и остроумный роман, который раскрывает пошехонский мир с его безумием, честностью и характером людей.', 1931, 'resources/imgBooks/Золотой теленок.jpg', 25),
	(9, 'Вишневый сад', 'В деревне Руссияннов происходят судьбоносные события. Любовь, смерть, утраты и иллюзии - все это переплетается в жизни героев, среди которых барыня Раневская, которая возвращается на родные земли, и ее племянница, проницательная Варя. Чеховская пьеса "Вишневый сад" приглашает читателя задуматься о временности существования, об очаровании и обреченности прошлого, а также о смысле и ценности жизни.', 1904, 'resources/imgBooks/Вишневый сад.jpg', 30),
	(10, 'Портрет Дориана Грея', 'В настоящем шедевре британской литературы Оскар Уайльд задает вопросы о красоте, отражении морали и ценой бессмертия. Путешествие по пути самовоскрешения и безрассудной эстетики, история о Дориане Грее и его портрете, который стареет откровенно, неизменно оставляет своих героев в плену собственных желаний и сомнений. "Портрет Дориана Грея" призывает нас подумать о цене красоты и последствиях внутреннего преображения.', 1890, 'resources/imgBooks/Дориан Грей.jpg', 30),
	(11, 'Гордость и предубеждение', 'В мире аристократии, где общество определяется состоянием, Элизабет Беннет и главный герой - гордый мистер Дарси - так и не могут сдружиться. Однако, в ходе сплетен, мнимых предубеждений и неожиданных открытий, они обнаруживают, что мнение о людях иногда может быть неверным. "Гордость и предубеждение" - это великолепный роман о любви и преодолении различий, который вечен и актуален до сих пор.', 1813, 'resources/imgBooks/Гордость и предубеждение.jpg', 30),
	(12, 'Старик и море', 'Глубокая повесть, пронизанная силой, отчаянием и надеждой. В небольшой кубинской деревне живет Сантьяго, старый рыбак, который уже долго не смог поймать достойную добычу. Но вдруг настанет день, когда он отправится в открытое море и попытается справиться с огромным марлином. Боязнь, тяжелый труд и преодоление собственных границ - все это будет сопровождать старика в его борьбе за достойное место в великой душе океана.', 1952, 'resources/imgBooks/Старик и море.jpg', 30),
	(13, 'Великий Гэтсби', 'В роскошном Нью-Йорке 1920-х годов, за многогранными тусовками и блеском вечеринок, скрывается история о страсти, роскоши и разочарованиях. Гэтсби, таинственный миллионер, стремится вернуть свою потерянную любовь, которая живет в соседнем особняке. Со сложными персонажами и неожиданными поворотами сюжета, "Великий Гэтсби" пленяет своей непреодолимой атмосферой эпохи и предупреждает о цене мечты, стремления к иллюзиям и пустоте благополучия.', 1925, 'resources/imgBooks/Гэтсби.jpg', 30),
	(14, 'Янки из Коннектикута при дворе короля Артура', 'Знаменитый роман американского писателя Марка Твена о необычном временном путешествии, которое переносит главного героя, инженера по профессии Генкса Моргана, американца XIX века, в Англию времен короля Артура. Главный герой оказывается в средневековом Королевстве Кэмелот, где обнаруживает многое, что противоречит его современным представлениям о жизни. Роман является уморительной сатирой на воззрения и поведение личностей двух эпох, а также на социальные и политические системы.', 1889, 'resources/imgBooks/Янки из Коннектикута при дворе короля Артура.jpeg', 21);

-- Дамп структуры для таблица diglib.books_authors
CREATE TABLE IF NOT EXISTS `books_authors` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `book_id` int unsigned NOT NULL,
  `author_id` int unsigned DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `book_id` (`book_id`),
  KEY `author_id` (`author_id`),
  CONSTRAINT `books_authors_ibfk_1` FOREIGN KEY (`book_id`) REFERENCES `books` (`id`) ON DELETE CASCADE,
  CONSTRAINT `books_authors_ibfk_2` FOREIGN KEY (`author_id`) REFERENCES `authors` (`id`) ON DELETE SET NULL
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Дамп данных таблицы diglib.books_authors: ~17 rows (приблизительно)
INSERT INTO `books_authors` (`id`, `book_id`, `author_id`) VALUES
	(1, 1, 1),
	(2, 2, 2),
	(3, 3, 3),
	(4, 4, 4),
	(5, 5, 5),
	(6, 6, 6),
	(7, 7, 7),
	(8, 7, 8),
	(9, 8, 7),
	(10, 8, 8),
	(11, 9, 9),
	(12, 10, 10),
	(13, 11, 11),
	(14, 12, 12),
	(15, 13, 13),
	(18, 14, 4);

-- Дамп структуры для таблица diglib.books_genres
CREATE TABLE IF NOT EXISTS `books_genres` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `book_id` int unsigned DEFAULT NULL,
  `genre_id` int unsigned DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `book_id` (`book_id`),
  KEY `genre_id` (`genre_id`),
  CONSTRAINT `books_genres_ibfk_1` FOREIGN KEY (`book_id`) REFERENCES `books` (`id`) ON DELETE CASCADE,
  CONSTRAINT `books_genres_ibfk_2` FOREIGN KEY (`genre_id`) REFERENCES `genres` (`id`) ON DELETE SET NULL
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Дамп данных таблицы diglib.books_genres: ~23 rows (приблизительно)
INSERT INTO `books_genres` (`id`, `book_id`, `genre_id`) VALUES
	(1, 1, 1),
	(2, 2, 1),
	(3, 2, 5),
	(4, 3, 1),
	(5, 3, 4),
	(6, 4, 2),
	(7, 5, 3),
	(8, 6, 1),
	(9, 6, 5),
	(10, 7, 1),
	(11, 7, 5),
	(12, 8, 1),
	(13, 8, 5),
	(14, 9, 1),
	(15, 9, 7),
	(16, 9, 6),
	(17, 10, 1),
	(18, 10, 5),
	(19, 11, 1),
	(20, 11, 5),
	(21, 12, 1),
	(22, 12, 5),
	(23, 13, 5),
	(24, 14, 2),
	(25, 14, 5);

-- Дамп структуры для таблица diglib.books_types
CREATE TABLE IF NOT EXISTS `books_types` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `book_id` int unsigned NOT NULL,
  `type` enum('Аудиокнига','Печатный формат','Электронная версия') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `total` mediumint unsigned DEFAULT NULL,
  `in_stock` mediumint unsigned DEFAULT NULL,
  `file_path` varchar(150) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `book_id` (`book_id`),
  CONSTRAINT `books_types_ibfk_1` FOREIGN KEY (`book_id`) REFERENCES `books` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Дамп данных таблицы diglib.books_types: ~28 rows (приблизительно)
INSERT INTO `books_types` (`id`, `book_id`, `type`, `total`, `in_stock`, `file_path`) VALUES
	(1, 1, 'Печатный формат', 100, 100, NULL),
	(2, 1, 'Электронная версия', 70, 70, 'resources/eBooks/Евгений Онегин.txt'),
	(3, 2, 'Печатный формат', 25, 25, NULL),
	(4, 2, 'Аудиокнига', 20, 19, 'resources/audioBooks/...'),
	(5, 2, 'Электронная версия', 40, 40, 'resources/eBooks/Война и мир. Том 1.txt'),
	(6, 3, 'Печатный формат', 65, 64, NULL),
	(7, 3, 'Аудиокнига', 6, 5, 'resources/audioBooks/...'),
	(8, 3, 'Электронная версия', 10, 9, 'resources/eBooks/Преступление и наказание.txt'),
	(9, 4, 'Печатный формат', 25, 23, NULL),
	(10, 5, 'Аудиокнига', 5, 4, 'resources/audioBooks/...'),
	(11, 5, 'Электронная версия', 10, 10, 'resources/eBooks/1984.txt'),
	(12, 6, 'Печатный формат', 10, 9, NULL),
	(13, 6, 'Аудиокнига', 5, 5, 'resources/audioBooks/...'),
	(14, 6, 'Электронная версия', 3, 2, 'resources/eBooks/Мастер и Маргарита.txt'),
	(15, 7, 'Печатный формат', 10, 8, NULL),
	(16, 8, 'Аудиокнига', 5, 5, 'resources/audioBooks/...'),
	(17, 8, 'Электронная версия', 3, 1, 'resources/eBooks/Золотой теленок.txt'),
	(18, 9, 'Аудиокнига', 5, 5, 'resources/audioBooks/...'),
	(19, 10, 'Печатный формат', 5, 4, NULL),
	(20, 10, 'Аудиокнига', 5, 3, 'resources/audioBooks/...'),
	(21, 11, 'Электронная версия', 7, 7, 'resources/eBooks/Гордость и предубеждение.txt'),
	(22, 12, 'Печатный формат', 4, 4, NULL),
	(23, 12, 'Аудиокнига', 10, 9, 'resources/audioBooks/...'),
	(24, 12, 'Электронная версия', 1, 0, 'resources/eBooks/Старик и море.txt'),
	(25, 13, 'Печатный формат', 5, 5, NULL),
	(26, 13, 'Аудиокнига', 5, 4, 'resources/audioBooks/...'),
	(27, 13, 'Электронная версия', 4, 3, 'resources/eBooks/Великий Гэтсби.txt'),
	(28, 14, 'Аудиокнига', 10, 9, 'resources/audioBooks/...');

-- Дамп структуры для таблица diglib.genres
CREATE TABLE IF NOT EXISTS `genres` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `genre` varchar(30) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `genre` (`genre`),
  FULLTEXT KEY `fti_genres` (`genre`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Дамп данных таблицы diglib.genres: ~7 rows (приблизительно)
INSERT INTO `genres` (`id`, `genre`) VALUES
	(4, 'Детектив'),
	(6, 'драма'),
	(1, 'Классика'),
	(7, 'Комедия'),
	(2, 'Приключения'),
	(5, 'Роман'),
	(3, 'Фантастика');

-- Дамп структуры для процедура diglib.returnBooksAfterIssueTime
DELIMITER //
CREATE PROCEDURE `returnBooksAfterIssueTime`()
BEGIN
	DECLARE _rollback BOOL DEFAULT FALSE;
	DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET _rollback = true;
	
	SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
	START TRANSACTION;
	
UPDATE books_types 
  		SET in_stock = in_stock + 1
  		WHERE books_types.id IN (SELECT * FROM
  		(select users_books.book_type_id FROM users_books 
		LEFT JOIN books_types ON users_books.book_type_id = books_types.id
		LEFT JOIN books ON books.id = books_types.book_id
		WHERE DATE_ADD(date_taken,INTERVAL cnt_days DAY) < CURDATE() AND date_return IS NULL AND books_types.type != 'Печатный формат'
		 ) AS FINAL);
  
    	UPDATE users_books 
		LEFT JOIN books_types ON users_books.book_type_id = books_types.id
		LEFT JOIN books ON books.id = books_types.book_id
		SET date_return = CURDATE()
		WHERE DATE_ADD(date_taken,INTERVAL cnt_days DAY) < CURDATE() AND date_return IS NULL AND books_types.type != 'Печатный формат' ;	
	
	IF _rollback THEN
		ROLLBACK;
	ELSE 
		COMMIT;
	END IF;
	
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
	
END//
DELIMITER ;

-- Дамп структуры для процедура diglib.returnBooksAfterReservedTime
DELIMITER //
CREATE PROCEDURE `returnBooksAfterReservedTime`()
BEGIN
	DECLARE cnt_days_reserved TINYINT DEFAULT 8;
	DECLARE _rollback BOOL DEFAULT FALSE;
	DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET _rollback = true;
	
	SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
	START TRANSACTION;
		
		UPDATE books_types SET in_stock = in_stock + 1
  		WHERE books_types.id IN (SELECT * FROM
  		(select users_books.book_type_id FROM users_books 
		WHERE DATE_ADD(date_reserv,INTERVAL cnt_days_reserved DAY) < CURDATE() AND date_return IS NULL
		 ) AS FINAL);
  
    	UPDATE users_books 
		LEFT JOIN books_types ON users_books.book_type_id = books_types.id
		LEFT JOIN books ON books.id = books_types.book_id
		SET date_return = CURDATE() AND date_taken = CURDATE()
		WHERE DATE_ADD(date_reserv,INTERVAL cnt_days_reserved DAY) < CURDATE() AND date_return IS NULL;	
	
	IF _rollback THEN
		ROLLBACK;
	ELSE 
		COMMIT;
	END IF;
	
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
	
END//
DELIMITER ;

-- Дамп структуры для таблица diglib.users
CREATE TABLE IF NOT EXISTS `users` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `first_name` varchar(30) NOT NULL,
  `last_name` varchar(30) NOT NULL,
  `isAdmin` tinyint(1) DEFAULT '0',
  `login` varchar(30) NOT NULL,
  `passwd` varchar(30) NOT NULL,
  `hashPass` varchar(64) NOT NULL,
  `salt` varchar(64) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Дамп данных таблицы diglib.users: ~35 rows (приблизительно)
INSERT INTO `users` (`id`, `first_name`, `last_name`, `isAdmin`, `login`, `passwd`, `hashPass`, `salt`) VALUES
	(1, 'Ян', 'Асман', 1, 'janasman17', 'superpass', 'e66d2158be6ec15ecf68e6ebc15c60f06a6c654af7bf816891714b04e62148bf', '4ZxNxQcTK6wHuSWtQO1jw6uznKB80zhQFRu1Ch7tP6lDNTuaRK21vSeCjUOTh4Cv'),
	(2, 'Кирилл', 'Федоров', 0, 'fedor12', 'kirill_fedorov', '0e33719cd464ea0c889cf9003419c0d21ac67a2879f06c200bc0a678eec2c11f', 'v1l5146W6VR9mqyiAPSszZli9zq9WKEanp1tfjSdEb6y048m65NYnQ1lXI4xCP40'),
	(3, 'Олег', 'Дмитриев', 0, 'katapulta', 'snegir', '53a1fd6bdb1822a04ec5d4bb2ed0a03166992a399ed9d8afd0b26212a838757c', 'OVC45MB5i4e2B9Q4eDibDFwVr7FYpiTv2YPnY9LEDsTT63bL9hEmsP8isxPX16MZ'),
	(4, 'Роман', 'Ефимов', 0, 'roman123', 'roman123', '883d929e649e595d925cde3d69ea6945d7623fe8a3a2a27c9416471b9708e66d', 'eozdWAkQxD4abTn0dcr6fi7ovhJsiohpbSDmmR0iYAHjfVJIO9XDkNbtzIKkh3O5'),
	(5, 'Артем', 'Шевердяев', 0, 'sheva007', 'mypassword', 'a06fe16120f1c2ce6e8248fb703b1fc8de2dda845886aa33740f80b66d84b42d', 'szJZHILXXXiP9w7gx4UjKxyQT1r9AmPeP7nyMZ8MPSBTcK3theuq4r5GZ3AbDyHV'),
	(6, 'Игорь', 'Иванов', 0, 'straznik', 'gorpun', '43ec2852f945051809c430ef40c26cd3f20872b6468d67cb9fc359e1f0e0b2a1', 'Gq9QBppX6tVpkmQFMfXvHFEQz1e3EXW4KrbIHh8nXfTXrCUMXhYQdpoc5fs18v5S'),
	(7, 'Димон', 'Алексеев', 0, 'dimaWin', 'dimaWin', 'ea8c587f8f12367994f0ea7adeba33db47fef20d3f208dcc44e03ba236be7f85', '0F55FRl7lM2jMMhXBW1HWU05UFq2ZGuq4LlL7upzDNyJhK3HrZUYYfHhSVFoBstX'),
	(8, 'Татьяна', 'Якименко', 0, 'tanya_kmn', 'klavdia', 'de0a3e161ff33b362862cb9d03390b26d98d7de42cc5c063ed4a199045028e99', 'XPKkGgTFQ5DpkVfe2pqgLvlLX88Pse82JUp6UUZVcjPom1ScMmtJQ45AWBzjR14T'),
	(9, 'Егор', 'Бут', 0, 'but000', 'but111', 'c0e3a82f577ca34c175f0c3f6d6ad857958432759760d3256a58856bfca3c50d', 'HSuIa6Q6dF2lL8b4BnlqiZn1kiyVLyhTe1GdXhKLmB0KfrsN9ly9BVL7Avy17HxF'),
	(10, 'Ян', 'Асман', 0, 'janasman', 'superpass', '933b02beeee919b372dedf8d9a93bc555ec5bf9c9e0d896bff7c40a911f6f8a5', '1hxnv4VaIzJCaHhdLU9e2Aoz4AG20lQKKnxDhvyQsyMZvVbNQyoSWTYeGyKJjDgT');

-- Дамп структуры для таблица diglib.users_books
CREATE TABLE IF NOT EXISTS `users_books` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int unsigned DEFAULT NULL,
  `book_type_id` int unsigned DEFAULT NULL,
  `date_reserv` date DEFAULT NULL,
  `date_taken` date DEFAULT NULL,
  `date_return` date DEFAULT NULL,
  `code` varchar(6) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `book_type_id` (`book_type_id`),
  KEY `ind_bktype_us` (`user_id`,`book_type_id`) USING BTREE,
  CONSTRAINT `users_books_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE,
  CONSTRAINT `users_books_ibfk_2` FOREIGN KEY (`book_type_id`) REFERENCES `books_types` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Дамп данных таблицы diglib.users_books: ~49 rows (приблизительно)
INSERT INTO `users_books` (`id`, `user_id`, `book_type_id`, `date_reserv`, `date_taken`, `date_return`, `code`) VALUES
	(1, 6, 15, '2023-12-23', '2023-12-23', NULL, '030282'),
	(2, 6, 9, '2023-12-23', NULL, NULL, '030318'),
	(3, 6, 18, NULL, '2023-12-23', '2023-12-23', NULL),
	(4, 6, 24, NULL, '2023-12-23', NULL, NULL),
	(5, 2, 28, NULL, '2023-12-23', NULL, NULL),
	(6, 2, 20, NULL, '2023-12-23', NULL, NULL),
	(7, 2, 14, NULL, '2023-12-23', NULL, NULL),
	(8, 3, 17, NULL, '2023-12-23', NULL, NULL),
	(9, 3, 10, NULL, '2023-12-23', NULL, NULL),
	(10, 5, 26, NULL, '2023-12-23', NULL, NULL),
	(11, 5, 27, NULL, '2023-12-23', NULL, NULL),
	(12, 5, 12, '2023-12-23', NULL, NULL, '030807'),
	(13, 7, 20, NULL, '2023-12-23', NULL, NULL),
	(14, 7, 23, NULL, '2023-12-23', NULL, NULL),
	(15, 7, 14, NULL, '2023-12-23', '2023-12-23', NULL),
	(16, 7, 19, '2023-12-23', NULL, NULL, '031170'),
	(17, 8, 26, NULL, '2023-12-23', '2023-12-23', NULL),
	(18, 8, 11, NULL, '2023-12-23', '2023-12-23', NULL),
	(19, 8, 17, NULL, '2023-12-23', NULL, NULL),
	(20, 8, 10, NULL, '2023-12-23', '2023-12-23', NULL),
	(21, 8, 21, NULL, '2023-12-23', '2023-12-23', NULL),
	(22, 9, 7, NULL, '2023-12-23', NULL, NULL),
	(23, 9, 6, '2023-12-23', NULL, NULL, '031712'),
	(24, 9, 8, NULL, '2023-12-23', NULL, NULL),
	(25, 10, 5, NULL, '2023-12-23', '2023-12-23', NULL),
	(26, 10, 4, NULL, '2023-12-23', NULL, NULL),
	(27, 10, 9, '2023-12-23', NULL, NULL, '032045'),
	(28, 10, 16, NULL, '2023-12-23', '2023-12-23', NULL);

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
