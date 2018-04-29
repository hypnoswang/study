--TEST--
xmlrpc_decode() Simple test decode type string

--CREDITS--
Michel Araujo <araujo_michel@yahoo.com.br>
#PHPSP 2013-08-22

--SKIPIF--
<?php if (!extension_loaded("xmlrpc")) print "skip"; ?>

--FILE--
<?php

$xml = <<<XML
<?xml version="1.0" encoding="utf-8"?>
<params>
<param>
 <value>
  <string>Is string</string>
 </value>
</param>
</params>
XML;

$response = xmlrpc_decode($xml);
echo $response;

--EXPECT--
Is string