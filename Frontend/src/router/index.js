import Vue from 'vue'
import Router from 'vue-router'

Vue.use(Router)

export default new Router({
    routes: [
        {
            path: '/',
            redirect: '/Environment'
        },
        {
            path: '/Problem',
            name: 'Problem',
            component: () => import('@/views/Problem/Problem.vue'),
            children: [
                {
                    path: '/Problem/list',
                    name: 'ProblemList',
                    component: () => import('@/views/Problem/List.vue'),
                    query: {
                        page: 1,
                        limit: 10
                    }
                },
                {
                    path: '/Problem/Detail',
                    name: 'ProblemDetail',
                    component: () => import('@/views/Problem/Detail.vue'),
                },
                {
                    path: '/Problem/Submit',
                    name: 'ProblemSubmit',
                    component: () => import('@/views/Problem/Submit.vue'),
                },
                {
                    path: '/',
                    redirect: '/Problem/list',
                }
            ]
        }
    ]
})