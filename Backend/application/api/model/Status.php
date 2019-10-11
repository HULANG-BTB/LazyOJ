<?php
/**
 * FileName: Status.php
 * Product: Lazy OJ
 * Author: Hu Lang
 * DateTime: 2019/9/27 13:08
 * Email: admin@oibit.cn
 * GitHub: https://github.com/HULANG-BTB
 * Description:
 *
 */


namespace app\api\model;


use think\Model;

class Status extends Model
{

    protected $pk = 'sid';
    protected $table = 'solution';
    protected $autoWriteTimestamp = 'datetime';

    public function getUidAttr($value) {
        $user = new User();
        return $user->get($value)->username;
    }

    public function getResultAttr($value)
    {
        $map = [
            0 => '等待',
            1 => '等待重判',
            2 => '编译中',
            3 => '评测中',
            4 => '正确',
            5 => '格式错误',
            6 => '答案错误',
            7 => '时间超限',
            8 => '内存超限',
            9 => '输出超限',
            10 => '运行错误',
            11 => '编译错误'
        ];
        return $map[$value];
    }

    public function getLanguageAttr($value) {
        $language = new Language();
        return $language->get($value)->title;
    }

    public function setLanguageAttr($value) {
        $language = new Language();
        return $language->where(['title' => $value])->find()->lid;
    }

}
