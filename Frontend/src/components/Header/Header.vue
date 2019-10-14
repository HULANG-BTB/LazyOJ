<template>
  <div class="header">
    <div class="logo">
      <div class="img"></div>
      <div class="title">Lazy OJ</div>
    </div>
    <div class="nav">
      <el-menu class="el-menu-demo" mode="horizontal" background-color="#409EFF" text-color="#fff">
        <el-submenu index="1">
          <template slot="title">
            <i class="el-icon-s-promotion"></i>
            <span>消息</span>
          </template>
          <el-menu-item index="1-1">没有新消息</el-menu-item>
        </el-submenu>
        <el-menu-item index="2" @click="loginDialogVisible = true" v-if="!loginStatus.username">
          <a>登录</a>
        </el-menu-item>
        <el-menu-item index="3" @click="registerDialogVisible = true" v-if="!loginStatus.username">
          <a>注册</a>
        </el-menu-item>
        <!--    登陆后显示    -->
        <el-submenu index="2" v-if="loginStatus.username">
          <template slot="title">
            <el-avatar :size="30" src="https://cube.elemecdn.com/3/7c/3ea6beec64369c2642b92c6726f1epng.png"></el-avatar>
            <a>{{ loginStatus.username }}</a>
          </template>
          <el-menu-item index="2-1">个人中心</el-menu-item>
          <el-menu-item index="2-2" @click="logoutSubmit">注销登录</el-menu-item>
        </el-submenu>
      </el-menu>
    </div>
    <Login v-model="loginDialogVisible"></Login>
    <Register v-model="registerDialogVisible"></Register>
  </div>
</template>

<script>
  import Login from "../Login/Login";
  import Register from "../Login/Register";
  import {
    mapState,
    mapActions
  } from 'vuex'
  import {
    reqLogoutSubmit
  } from '../../api'
  export default {
    data () {
      return {
        loginDialogVisible: false,
        registerDialogVisible: false,
      }
    },
    computed: {
      ...mapState(['userInfo']),
      loginStatus () {
        return this.userInfo
      }
    },
    methods: {
      ...mapActions(['getUserInfo']),
      async logoutSubmit () {
        const result = await reqLogoutSubmit()
        if (result.code === 1) {
          this.$message('注销成功')
          this.getUserInfo({})
        }
      }
    },
    components: {
      Login,
      Register
    }
  }
</script>

<style scoped>
.header > .logo {
  padding-top: 10px;
  padding-left: 10px;
  float: left;
}
.header > .logo .img {
  width: 30px;
  height: 30px;
  float: left;
}
.header > .logo .title {
  font-family: "MV Boli";
  font-size: 24px;
  float: left;
}
.header > .nav {
  float: right;
}
.header i {
  color: black;
}
</style>
