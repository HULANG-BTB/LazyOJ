<template>
    <el-container>
        <el-header :style="{height: 'initial'}">
            <el-row style="text-align: center;">
                <el-row>
                    <h1>环境和编译选项</h1>
                </el-row>
            </el-row>
        </el-header>
        <el-main>
            <el-table
                    :data="languageList.data"
                    style="width: 100%"
                    border
                    fit
            >
                <el-table-column
                        label="编号"
                        width="100">
                    <template slot-scope="scope">
                        <span style="margin-left: 10px">{{ scope.row.lid }}</span>
                    </template>
                </el-table-column>
                <el-table-column
                        label="语言"
                        width="200">
                    <template slot-scope="scope">
                        <span style="margin-left: 10px">{{ scope.row.title }}</span>
                    </template>
                </el-table-column>
                <el-table-column
                        label="命令">
                    <template slot-scope="scope">
                        <span style="margin-left: 10px">{{ scope.row.cmd }}</span>
                    </template>
                </el-table-column>
            </el-table>
        </el-main>

    </el-container>
</template>

<script>
    import {
        mapState,
        mapActions
    } from 'vuex'
    export default {
        name: 'Environment',
        data () {
             return {

             }
        },
        computed: {
            ...mapState(['languageList'])
        },
        methods: {
            ...mapActions(['getLanguageList'])
        },
        mounted() {
            this.getLanguageList({})
        }

    }
</script>

<style>

</style>
