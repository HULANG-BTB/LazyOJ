/*
captcha请求函数模块
返回值 promise对象（response.data）
*/

import axios from 'axios'

axios.defaults.withCredentials=true;

export default function captcha (url = '') {
    return new Promise(function (resolve, reject) {
        // 执行异步ajax请求
        let promise
        promise = axios.get(url, {responseType: 'blob'})
        promise.then(function (response) {
            // 成功调用resolve 返回数据
            let reader = new FileReader()
            reader.onload = (e) => {
                resolve(e.target.result)
            }
            reader.readAsDataURL(response.data)
        }).catch(function (error) {
            // 失败返回错误
            reject(error)
        })
    })
}
