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
        <el-col :span="4" ref="ref2222">
          <el-input size="medium" placeholder="请输入内容" suffix-icon="el-icon-date" v-model="input4"></el-input>
        </el-col>
        <el-col :span="4" ref="ref1111">
          <el-button type="primary" icon="el-icon-search" @click="showVersionDialog()">搜索</el-button>
        </el-col>
      </div>
    </el-header>
    <el-main>
      <el-table :data="tableData" style="width: 100%" height="400">
        <el-table-column fixed prop="date" label="日期" width="150"></el-table-column>
        <el-table-column prop="name" label="游戏名称" width="120"></el-table-column>
        <el-table-column prop="VersionName" label="版本名称" width="120"></el-table-column>
        <el-table-column prop="lookover" label="查看" width="120"></el-table-column>
      </el-table>
    </el-main>
    <add-or-update v-if="versionEditDialogVis" ref="VersionEditDialog"></add-or-update>
  </el-container>
</template>
<script>
import VersionEditDialog from "./Dialog/VersionEditDialog.vue";
// Vue.component('add-or-update', require('./add.vue').default)
export default {
  name: "GameVersion",
  components: {
    "add-or-update":VersionEditDialog
  },
  data() {
    return {
      input1: "",
      input2: "",
      input3: "",
      input4: "",
      versionEditDialogVis: false,
      tableData: [
        {
          date: "2016-05-03",
          name: "王小虎",
          VersionName: "上海",
          lookover: "查看"
        },
        {
          date: "2016-05-03",
          name: "王小虎",
          VersionName: "上海",
          lookover: "查看"
        },
        {
          date: "2016-05-03",
          name: "王小虎",
          VersionName: "上海",
          lookover: "查看"
        },
        {
          date: "2016-05-03",
          name: "王小虎",
          VersionName: "上海",
          lookover: "查看"
        },
        {
          date: "2016-05-03",
          name: "王小虎",
          VersionName: "上海",
          lookover: "查看"
        },
        {
          date: "2016-05-03",
          name: "王小虎",
          VersionName: "上海",
          lookover: "查看"
        },
        {
          date: "2016-05-03",
          name: "王小虎",
          VersionName: "上海",
          lookover: "查看"
        }
      ]
    };
  },
  methods: {
    onBtnClick: function() {},

    showVersionDialog: function() {
      console.log("showVersionDialog");
      this.versionEditDialogVis = true;
      var dialog = this.$refs["VersionEditDialog"];
      if(dialog)dialog.init();
    }
  }
};
</script>
