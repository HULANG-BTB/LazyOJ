<?php

namespace app\api\model;

use think\Model;

class User extends Model
{
    //
    protected $autoWriteTimestamp = 'datetime';
    protected $pk = 'uid';
}
