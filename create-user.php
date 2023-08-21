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

if ($json!=null) {
    echo "json received...";
    $user_firstname = $json["user_firstname"];
    $user_lastname  = $json["user_lastname"];
    $user_rfid_uid  = $json["user_rfid_uid"];
    $user_pin       = $json["user_pin"];
    $relation_id    = $json["relation_id"];
    $active         = $json["active"];

    $sqlQuery = "INSERT INTO users (user_name, user_rfid_uid, user_pin, active) 
            VALUES (?,?,?,?)";
    $statement = $connection->prepare($sqlQuery);
    // Bind_param sssi = string, string, string, integer
    $statement->bind_param("sssi", $user_name, $user_rfid_uid, $user_pin, $active);

    // Execute query
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