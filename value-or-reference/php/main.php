<?php

class MyStruct {
    public $field;
}

function modify_single_value($single)
{
    $single = 2;
}

function modify_single_reference(&$single)
{
    $single = 2;
}

function modify_list_value($list)
{
    $list[1] = 2;
}

function modify_struct_value($struct)
{
    $struct->field = 2;
}

$local_variable = 1;
modify_single_value($local_variable);
var_dump($local_variable); // 1

$local_variable = 1;
modify_single_reference($local_variable);
var_dump($local_variable); // 2

$local_variable = [1, 1];
modify_list_value($local_variable);
var_dump($local_variable); // [1, 1]

$local_variable = new MyStruct();
$local_variable->field = 1;
modify_struct_value($local_variable);
var_dump($local_variable); // 2, 和 array 不同，object 是不拷贝的
