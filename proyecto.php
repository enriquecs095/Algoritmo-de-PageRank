<?php
if(isset($_POST['buscar'])){
$texto=$_POST['buscar'];
system("Proyecto1Estructura1.exe $texto");
}
?>