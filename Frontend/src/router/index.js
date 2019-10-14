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
            path: '/Environment',
            name: 'Environment',
            component: () => import('@/views/Environment/Environment.vue')
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
        },
        {
            path: '/Status',
            name: 'Status',
            component: () => import('@/views/Status/Status.vue'),
            children: [
                {
                    path: '/Status/List',
                    name: 'StatusList',
                    component: () => import('@/views/Status/List.vue')
                },
                {
                    path: '/Status',
                    redirect: '/Status/List',
                    query: {
                        page: 1,
                        limit: 10
                    }
                }
            ]
        }
    ]
})