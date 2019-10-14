<template>
    <div class="login">
        <el-dialog
            title="登录"
            :visible.sync="visiable"
            width="30%"
            center
            @close="closed"
        >
            <el-form :model="loginForm" label-width="100px">
                <el-form-item label="用户名">
                    <el-input v-model="loginForm.username" autocomplete="off"></el-input>
                </el-form-item>
                <el-form-item label="密码">
                    <el-input v-model="loginForm.password" autocomplete="off" type="password"></el-input>
                </el-form-item>
                <el-form-item label="验证码">
                    <el-input v-model="loginForm.captcha">
                        <template slot="append"><img :src="captchaSrc" alt="" height="34px" @click="getCaptcha">
                        </template>
                    </el-input>
                </el-form-item>
            </el-form>
            <span slot="footer" class="dialog-footer">
        <el-button type="primary" @click="loginSubmit">登 录</el-button>
        <el-button @click="closed">取 消</el-button>
      </span>
        </el-dialog>
    </div>
</template>

<script>
    import {
        mapActions,
        mapState
    } from 'vuex'

    import {
        reqCaptchaImg,
        reqLoginSubmit
    } from '../../api'

    export default {
        name: "Login",
        data() {
            return {
                loginForm: {
                    username: '',
                    password: '',
                    captcha: ''
                },
                captchaSrc: ''
            }
        },
        props: {
            value: {
                type: Boolean,
                default() {
                    return false
                }
            }
        },
        computed: {
            ...mapState(['userInfo']),
            visiable : {
                get () {
                    return this.value
                },
                set () {
                    return false
                }
            }
        },
        methods: {
            ...mapActions(['getCaptchaImg', 'getUserInfo']),
            async getCaptcha() {
                this.captchaSrc = await reqCaptchaImg(new Date().getTime())
            },
            async loginSubmit() {
                // this.loginDialogVisible = false
                const result = await reqLoginSubmit({
                    username: this.loginForm.username,
                    password: this.loginForm.password,
                    captcha: this.loginForm.captcha
                })
                if (result.code === 1) {
                    this.$message('登录成功')
                    this.getUserInfo({})
                    this.closed()
                } else {
                    this.$message(result.msg)
                    this.getCaptcha()
                }
            },
            closed () {
                this.$emit('input', false)
            }
        },
        mounted() {
            this.getCaptcha()
        }
    }
</script>

<style scoped>

</style>