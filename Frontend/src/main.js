import Vue from 'vue'
import App from './App'

import router from './router'
import store from './store'

import ElementUI from 'element-ui'
import 'element-ui/lib/theme-chalk/index.css'
import './assets/css/app.css'

import codemirror from 'vue-codemirror'
import 'codemirror/lib/codemirror.css'

Vue.use(ElementUI)
Vue.use(codemirror)

Vue.config.productionTip = false

new Vue({
  render: h => h(App),
  router,
  store
}).$mount('#app')
