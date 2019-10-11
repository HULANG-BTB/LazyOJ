<?php
/**
 * FileName: Language.php
 * Product: LazyOJ
 * Author: Hu Lang
 * DateTime: 2019/10/10 14:59
 * Email: admin@oibit.cn
 * GitHub: https://github.com/HULANG-BTB
 * Description:
 *
 */


namespace app\api\controller;

use app\api\model\Language as LanguageModel;
use app\common\controller\Base;
use think\App;

class Language extends Base
{
    public function __construct(App $app = null)
    {
        parent::__construct($app);
        $this->Model = new LanguageModel();
    }

    public function getLanguageList() {
        $result = $this->Model
            ->order('lid', 'asc')
            ->field(['lid', 'title'])
            ->select();
        $count = $this->Model->count();
        $ret = [
            'code' => 1,
            'data' => [
                'total' => $count,
                'data' => $result
            ]
        ];
        return json($ret);

    }

}
