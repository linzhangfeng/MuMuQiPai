<template>
  <el-container>
    <el-header style="text-align: right; font-size: 12px">
      <div class="demo-input-size">
        <el-col :span="4">
          <el-input size="medium" placeholder="请输入内容" suffix-icon="el-icon-date" v-model="input1"></el-input>
        </el-col>
        <el-col :span="4">
          <el-input size="medium" placeholder="请输入内容" suffix-icon="el-icon-date" v-model="input2"></el-input>
        </el-col>
        <el-col :span="4">
          <el-input size="medium" placeholder="请输入内容" suffix-icon="el-icon-date" v-model="input3"></el-input>
        </el-col>
        <el-col :span="4">
          <el-input size="medium" placeholder="请输入内容" suffix-icon="el-icon-date" v-model="input4"></el-input>
        </el-col>
        <el-col :span="4">
          <el-button type="primary" icon="el-icon-search">搜索</el-button>
        </el-col>
      </div>
    </el-header>
    <el-main>
      <el-table :data="tableData" style="width: 100%" height="400">
        <el-table-column fixed prop="date" label="日期" width="150"></el-table-column>
        <el-table-column prop="name" label="姓名" width="120"></el-table-column>
        <el-table-column prop="province" label="省份" width="120"></el-table-column>
        <el-table-column prop="city" label="市区" width="120"></el-table-column>
        <el-table-column prop="address" label="地址" width="300"></el-table-column>
        <el-table-column prop="zip" label="邮编" width="120"></el-table-column>
      </el-table>
    </el-main>
  </el-container>
</template>
<script>
export default {
  name: "UserInfo",
  data() {
    this.get();
    return {
      input1: "",
      input2: "",
      input3: "",
      input4: "",
      tableData: [
        {
          date: "2016-05-03",
          name: "王小虎",
          province: "上海",
          city: "普陀区",
          address: "上海市普陀区金沙江路 1518 弄",
          zip: 200333
        },
        {
          date: "2016-05-02",
          name: "王小虎",
          province: "上海",
          city: "普陀区",
          address: "上海市普陀区金沙江路 1518 弄",
          zip: 200333
        },
        {
          date: "2016-05-04",
          name: "王小虎",
          province: "上海",
          city: "普陀区",
          address: "上海市普陀区金沙江路 1518 弄",
          zip: 200333
        },
        {
          date: "2016-05-01",
          name: "王小虎",
          province: "上海",
          city: "普陀区",
          address: "上海市普陀区金沙江路 1518 弄",
          zip: 200333
        },
        {
          date: "2016-05-08",
          name: "王小虎",
          province: "上海",
          city: "普陀区",
          address: "上海市普陀区金沙江路 1518 弄",
          zip: 200333
        },
        {
          date: "2016-05-06",
          name: "王小虎",
          province: "上海",
          city: "普陀区",
          address: "上海市普陀区金沙江路 1518 弄",
          zip: 200333
        },
        {
          date: "2016-05-07",
          name: "王小虎",
          province: "上海",
          city: "普陀区",
          address: "上海市普陀区金沙江路 1518 弄",
          zip: 200333
        }
      ]
    };
  },
  methods: {
    get(){
      this.$http({
        method: "get",
        url: "/api/getDicType"
      })
        .then(response => {
          alert("hello,");
          var _data = response.data;
          alert("hello," + _data.username);
        })
        .catch(function(err) {
          console.log(err);
        });
    }
  }
};
</script>
