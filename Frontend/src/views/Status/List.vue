<template>
  <el-container>
    <el-header :style="{height: 'initial'}">
      <el-row style="text-align: center;">
        <el-row>
          <h1>状态</h1>
          <el-form :inline="true" class="demo-form-inline">
            <el-form-item label="题号">
              <el-input v-model="id" placeholder="题号"></el-input>
            </el-form-item>
            <el-form-item label="用户">
              <el-input v-model="user" placeholder="用户"></el-input>
            </el-form-item>
            <el-form-item label="语言">
              <el-select v-model="language" placeholder="语言">
                <el-option label="ALL" value="-1"></el-option>
                <el-option label="C" value="0"></el-option>
                <el-option label="C++" value="1"></el-option>
                <el-option label="Java" value="3"></el-option>
                <el-option label="Python" value="6"></el-option>
                <el-option label="Sql" value="18"></el-option>
              </el-select>
            </el-form-item>
            <el-form-item label="结果">
              <el-select v-model="result" placeholder="结果">
                <el-option label="ALL" value="-1"></el-option>
                <el-option label="正确" value="4"></el-option>
                <el-option label="格式错误" value="5"></el-option>
                <el-option label="答案错误" value="6"></el-option>
                <el-option label="时间超限" value="7"></el-option>
                <el-option label="内存超限" value="8"></el-option>
                <el-option label="输出超限" value="9"></el-option>
                <el-option label="运行错误" value="10"></el-option>
                <el-option label="编译错误" value="11"></el-option>
                <el-option label="等待" value="0"></el-option>
                <el-option label="等待重判" value="1"></el-option>
                <el-option label="编译中" value="2"></el-option>
                <el-option label="评测中" value="3"></el-option>
              </el-select>
            </el-form-item>
            <el-form-item>
              <el-button type="primary" @click.prevent="search">查询</el-button>
            </el-form-item>
          </el-form>
        </el-row>
      </el-row>
    </el-header>
    <el-main>
      <el-table
        :data="tableData"
        style="width: 100%"
        border
        fit
      >
        <el-table-column
          label="编号"
          width="100">
          <template slot-scope="scope">
            <span style="margin-left: 10px">{{ scope.row.sid }}</span>
          </template>
        </el-table-column>
        <el-table-column
          label="用户"
          width="150">
          <template slot-scope="scope">
            <span style="margin-left: 10px">{{ scope.row.uid }}</span>
          </template>
        </el-table-column>
        <el-table-column
          label="问题"
          width="100">
          <template slot-scope="scope">
            <span style="margin-left: 10px" @click="goToProblem(scope.row.pid)">{{ scope.row.pid }}</span>
          </template>
        </el-table-column>
        <el-table-column
          label="结果"
          width="160">
          <template slot-scope="scope">
            <span style="margin-left: 10px" v-bind:style="{color: getResultStyle(scope.row.result)}">{{ scope.row.result }}</span>
          </template>
        </el-table-column>
        <el-table-column
          label="内存"
          width="130">
          <template slot-scope="scope">
            <span style="margin-left: 10px">{{ scope.row.memory / 1024 }} KB</span>
          </template>
        </el-table-column>
        <el-table-column
          label="耗时"
          width="130">
          <template slot-scope="scope">
            <span style="margin-left: 10px">{{ scope.row.time }} MS</span>
          </template>
        </el-table-column>
        <el-table-column
          label="语言">
          <template slot-scope="scope">
            <span style="margin-left: 10px">{{ scope.row.language }}</span>
          </template>
        </el-table-column>
        <el-table-column
          label="代码长度">
          <template slot-scope="scope">
            <span style="margin-left: 10px">{{ scope.row.length }} B</span>
          </template>
        </el-table-column>
        <el-table-column
          label="提交时间"
          width="200">
          <template slot-scope="scope">
            <span style="margin-left: 10px">{{ scope.row.create_time }}</span>
          </template>
        </el-table-column>
        <el-table-column
          label="评测"
          width="80">
          <template slot-scope="scope">
            <span style="margin-left: 10px">{{ scope.row.judger }}</span>
          </template>
        </el-table-column>
      </el-table>
    </el-main>
    <el-footer>
      <el-pagination
        @size-change="handleSizeChange"
        @current-change="handlePageChange"
        :current-page="page"
        :page-sizes="[10, 20, 50, 100]"
        :page-size="limit"
        layout="total, sizes, prev, pager, next, jumper"
        :total="total">
      </el-pagination>
    </el-footer>
  </el-container>
</template>

<script>
import {
  mapActions,
  mapState
} from 'vuex'

export default {
  name: 'StatusList',
  data () {
    return {
      page: parseInt(this.$route.query.page) || 1,
      limit: parseInt(this.$route.query.limit) || 10,
      id: this.$route.query.id || '',
      user: this.$route.query.user || '',
      language: this.$route.query.language || 'ALL',
      result: this.$route.query.result || 'ALL',
      timer: null
    }
  },
  computed: {
    tableData () {
      return this.statusList.data
    },
    ...mapState(['statusList']),
    total () {
      return this.statusList.total
    }
  },
  mounted () {
    const query = this.buildQuery()
    this.getStatusList(query)
  },
  methods: {
    ...mapActions(['getStatusList']),
    handlePageChange (data) {
      this.page = data
      const query = this.buildQuery()
      this.$router.push({name: 'StatusList', query: query})
      this.getStatusList(query)
    },
    handleSizeChange (data) {
      this.limit = data
      const query = this.buildQuery()
      this.$router.push({name: 'StatusList', query: query})
      this.getStatusList(query)
    },
    redirect (obj) {
      this.$router.push({name: 'ProblemDetail', query: {id: obj.id}})
    },
    search () {
      this.page = 1
      const query = this.buildQuery()
      this.$router.push({name: 'StatusList', query: query})
      this.getStatusList(query)
    },
    goToProblem (pid) {
      this.$router.push({name: 'ProblemDetail', query: {id: pid}})
    },
    buildQuery () {
      return {
        page: this.page,
        limit: this.limit,
        id: this.id,
        user: this.user,
        language: this.language,
        result: this.result
      }
    },
    getResultStyle (result) {
      switch (result) {
        case '正确':
          return '#5cb85c'
        case '格式错误':
          return '#c9302c'
        case '答案错误':
          return '#c9302c'
        case '时间超限':
          return '#ec971f'
        case '内存超限':
          return '#ec971f'
        case '输出超限':
          return '#ec971f'
        case '运行错误':
          return '#ec971f'
        case '编译错误':
          return '#ec971f'
        default :
          return '#ec971f'
      }
    }
  }
}
</script>

<style scoped>
.el-select {
  width: 150px;
}
</style>
