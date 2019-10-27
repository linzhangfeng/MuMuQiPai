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
        <el-col :span="4">
          <el-button type="primary" icon="el-icon-search">新增类型</el-button>
        </el-col>
      </div>
    </el-header>
    <el-main>
      <el-table :data="tableData" style="width: 100%" height="400">
        <el-table-column fixed prop="dictype_id" label="字典类型ID" width="150"></el-table-column>
        <el-table-column prop="dictype_name" label="字典类型名称" width="240"></el-table-column>
        <el-table-column prop="date" label="创建时间" width="240"></el-table-column>
      </el-table>
    </el-main>
  </el-container>
</template>
<script>
import moment from "moment";
export default {
  name: "UserInfo",
  data() {
    return {
      input1: "",
      input2: "",
      input3: "",
      input4: "",
      tableData: []
    };
  },
  mounted() {
    this.getTableData();
  },
  methods: {
    getTableData() {
      this.$http({
        method: "get",
        url: "/api/getDicType"
      })
        .then(response => {
          console.log("lin=data=response:" + JSON.stringify(response.data));
          var recvData = response.data.data;
          recvData = JSON.parse(recvData);
          var list = recvData["list"];
          console.log("lin=data=response:" + JSON.stringify(list));
          var tableData = [];
          for (var i = 0; i < list.length; i++) {
            var obj = list[i];
            var newObj = {};
            newObj["dictype_id"] = obj["dictype_id"];
            newObj["dictype_name"] = obj["dictype_name"];
            newObj["date"] = moment(new Date(obj["dictype_createtime"])).format(
              "YYYY-MM-DD HH:mm:ss"
            );
            tableData.push(newObj);
          }
          this.tableData = tableData;
        })
        .catch(function(err) {
          console.log(err);
        });
    }
  }
};
</script>
