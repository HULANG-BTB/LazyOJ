module.exports = {
    devServer: {
        proxy: {
            '/api': {
                target: 'http://lazyoj.cn',
                changeOrigin: true,
                pathRewrite: {
                    '^/api': ''
                }
            }
        }
    }
}