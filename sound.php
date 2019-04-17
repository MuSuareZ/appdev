<?php
$log_file = 'testcurl.txt';
$values = $_POST['db'];
$str = gmdate("M d Y H:i:s",time()).' '.$values."\n";

if(file_exists($log_file)) {
		$fp = fopen($log_file,a);
		fwrite($fp, $str);
		fclose($fp);
} else {
		$fp = fopen($log_file,w);
		fwrite($fp, $str);
		fclose($fp);
}
?>