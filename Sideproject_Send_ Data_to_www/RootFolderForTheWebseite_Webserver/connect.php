<?php
class HandelDB
{
    private $ini = array();

    private function connectToDB()
    {
        $this->ini = parse_ini_file('../.ini');
        //print_r($this->ini);
        try {

            $this->conn = new PDO("mysql:host={$this->ini['servername']};dbname={$this->ini['db']}", $this->ini['username'], $this->ini['password']);
            // set the PDO error mode to exception
            $this->conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            //echo "Connected successfully";
        } catch (PDOException $e) {
            echo "Connection failed: " . $e->getMessage();
        }
        $this->ini = "";
    }

    private function select($sql)
    {
        try {
            $stmt = $this->conn->prepare($sql);
            $stmt->execute();
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            echo $e->getMessage();
        }
    }
    //close the DB connection
    private function disconectFromDB()
    {
        $sql = null;
        $stmt = null;
        $this->conn = null;
    }

    public function getResults($sql)
    {
        $this->connectToDB();
        return $this->select($sql);
        $this->disconectFromDB();
    }

    public function pushUpdate($timestamp, $temperature, $humidity)
    {
        $this->connectToDB();

        $sql = "INSERT INTO `tempLog` (`timeStamp`,`temperature`, `humidity`) 
		VALUES (:timeStamp,:temperature,:humidity)";

        try {
            $stmt = $this->conn->prepare($sql);
            $stmt->bindParam(':timeStamp', $timestamp);
            $stmt->bindParam(':temperature', $temperature);
            $stmt->bindParam(':humidity', $humidity);
            $stmt->execute();
        } catch (PDOException $e) {
            echo $e->getMessage();
        }

        $this->disconectFromDB();
    }
}
