<template>
    <div class="editor">
        <div class="toobar">
            <el-form :inline="true">
                <el-form-item label="语言：">
                    <el-select v-model="options.language" placeholder="请选择" @change="languageChange">
                        <el-option
                                v-for="language in languages"
                                :key="language.value"
                                :label="language.label"
                                :value="language.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="主题：">
                    <el-select v-model="options.theme" placeholder="请选择" @change="themeChange">
                        <el-option
                                v-for="theme in themes"
                                :key="theme.value"
                                :label="theme.label"
                                :value="theme.value">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
        </div>
        <div class="content">
            <div ref="editor" class="editor-element"></div>
        </div>
    </div>
</template>

<script>
    export default {
        name: "Editor",
        data () {
            return {
                code: this.value.code,
                language: this.value.language,

            }
        },
        props: {
            value: {
                type: Object,
                default () {
                    return {
                        code: '',
                        language: ''
                    }
                }
            },
            options: {
                type: Object,
                default() {
                    return {

                    }
                }
            }
        },
        computed: {
            languages () {

            }
        },
        methods: {

        }

    }
</script>

<style scoped>

</style>