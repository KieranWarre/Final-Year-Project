<?php

// database credentials
$servername = "localhost";
$username = "arduino-test";
$password = "arduino";
$dbname = "db_arduino_test";
// create connection
$connection = new mysqli($servername, $username, $password, $dbname);
// check connection
if (!$connection) {
    die("Connection failed: " . $connection->connect_error);
}

$user_rfid_uid = $_GET["user_rfid_uid"];

if ($user_rfid_uid) {
    // define query
    $sqlQuery = "SELECT * FROM users WHERE user_rfid_uid = $user_rfid_uid";
    
    // execute query
    $result = $connection->query($sqlQuery);

    if ($result->num_rows > 0) {
        // fetch record
        $record = $result->fetch_assoc();

        // send record as json object
        header('Content-Type: applicaiton/json');
        http_response_code(200); //OK
        echo json_encode($record);
    } else {
        http_response_code(500); //Internal error
        echo "Record not found";
    }
} else {
    http_response_code(404); //bad request
    echo "Missing RFID data";
}

$connection->close();

?>