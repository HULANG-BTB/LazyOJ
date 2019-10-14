<template>
    <div class="register">
        <el-dialog
                title="注册"
                :visible.sync="visiable"
                width="30%"
                center
                @close="closed"
        >
            <el-form :model="registerForm" label-width="100px">
                <el-form-item label="用户名">
                    <el-input v-model="registerForm.username" autocomplete="off"></el-input>
                </el-form-item>
                <el-form-item label="密码">
                    <el-input v-model="registerForm.password" autocomplete="off" type="password"></el-input>
                </el-form-item>
                <el-form-item label="重复密码">
                    <el-input v-model="registerForm.repassword" autocomplete="off" type="password"></el-input>
                </el-form-item>
                <el-form-item label="邮箱地址">
                    <el-input v-model="registerForm.email" autocomplete="off"></el-input>
                </el-form-item>
                <el-form-item label="学校">
                    <el-input v-model="registerForm.school" autocomplete="off"></el-input>
                </el-form-item>
                <el-form-item label="验证码">
                    <el-input v-model="registerForm.captcha">
                        <template slot="append"><img :src="captchaSrc" alt="" height="34px" @click="getCaptcha">
                        </template>
                    </el-input>
                </el-form-item>
            </el-form>
            <span slot="footer" class="dialog-footer">
        <el-button type="primary" @click="registerSubmit">登 录</el-button>
        <el-button @click="closed">取 消</el-button>
      </span>
        </el-dialog>
    </div>
</template>

<script>
    import {
        mapActions
    } from 'vuex'

    import {
        reqCaptchaImg,
        reqRegisterSubmit
    } from '../../api'

    export default {
        name: "Register",
        data () {
            return {
                registerForm: {
                    username: '',
                    password: '',
                    repassword: '',
                    school: '',
                    email: '',
                    captcha: ''
                },
                captchaSrc: ''
            }
        },
        props: {
            value: {
                type: Boolean,
                default () {
                    return false
                }
            }
        },
        computed: {
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
            closed () {
                this.$emit('input', false)
            },
            async registerSubmit () {
                const result = await reqRegisterSubmit(this.registerForm)
                if (result.code === 1) {
                    this.$message('注册成功')
                    this.getUserInfo({})
                    this.closed()
                } else {
                    this.$message(result.msg)
                }
                this.getCaptcha()
            }
        },
        mounted() {
            this.getCaptcha()
        }

    }
</script>

<style scoped>

</style>