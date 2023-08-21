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

// retrieve and decode json data from POST request
$json = json_decode(file_get_contents('php://input'), true);

if ($json != null) {
    echo "json recieved...";
    $currentDateTime = date("Y-m-d H:i:s");

    $user_id            = $json["user_id"];
    $datetime_created   = $currentDateTime;
    $confirmed          = false;

    $sqlQuery = "INSERT INTO alerts (user_id, datetime_created, confirmed) 
            VALUES ($user_id, $datetime_created, $confirmed)";

    $statement = $connection->prepare($sqlQuery);

    if ($statement->execute()) {
        // query successful
        http_response_code(200); //OK
        echo "Data inserted correctly"; 
    } else {
        // Error executing query
        http_response_code(500); //internal error
        echo "Error inserting data: " . $statement->error;
    }
} else {
    // json decode failed
    http_response_code(400);
    exit("Invalid JSON data");
}

// Close connections
$statement->close();
$connection->close();

?>