<?php
$username="citphoto_tuser";
$password="tuser";
$database="citphoto_trial";
$fn=$_POST['firstname'];
$ln=$_POST['lastname'];
$age=$_POST['age'];
mysql_connect(localhost,$username,$password);
@mysql_select_db($database) or die( "Unable to select database");
$query = "INSERT INTO Members(FirstName, LastName, Age) VALUES('$fn','$ln','$age')";
mysql_query($query);
mysql_close();
?>