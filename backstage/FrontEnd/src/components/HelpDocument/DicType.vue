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
          <el-button type="primary" icon="el-icon-search" @click="dialogFormVisible = true">新增类型</el-button>
        </el-col>
      </div>
    </el-header>
    <el-main>
      <el-table :data="tableData" style="width: 100%" height="400">
        <el-table-column fixed prop="dictype_id" label="字典类型ID" width="150"></el-table-column>
        <el-table-column prop="dictype_name" label="字典类型名称" width="240"></el-table-column>
        <el-table-column prop="date" label="创建时间" width="240"></el-table-column>
        <el-table-column prop="data" label="其他操作" width="240">
          <template slot-scope="scope">
            <el-button
              type="primary"
              icon="el-icon-edit"
              @click="updateDictype(scope.$index)"
              circle
            ></el-button>
            <el-button type="danger" icon="el-icon-delete" @click="deleteDictype(scope.row)" circle></el-button>
          </template>
        </el-table-column>
      </el-table>

      <el-dialog title="添加字典类型" :visible.sync="dialogFormVisible">
        <el-form :model="form">
          <el-form-item label="字典类型名称" :label-width="formLabelWidth">
            <el-input v-model="form.name" autocomplete="off"></el-input>
          </el-form-item>
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button @click="dialogFormVisible = false">取 消</el-button>
          <el-button type="primary" @click="addDictype">确 定</el-button>
        </div>
      </el-dialog>
      <el-dialog title="修改" :visible.sync="update_dialog_visible">
        <el-form :model="curItem">
          <el-form-item label="字典类型编码" :label-width="formLabelWidth">
            <el-input v-model="curItem.id" autocomplete="off"></el-input>
          </el-form-item>
          <el-form-item label="字典类型名称" :label-width="formLabelWidth">
            <el-input v-model="curItem.name" autocomplete="off"></el-input>
          </el-form-item>
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button @click="update_dialog_visible = false">取 消</el-button>
          <el-button type="primary" @click="updateDictype">确 定</el-button>
        </div>
      </el-dialog>
      <el-dialog title="提示" :visible.sync="addDialogTip" width="30%">
        <span>添加成功</span>
        <span slot="footer" class="dialog-footer">
          <el-button @click="addDialogTip = false">取 消</el-button>
          <el-button type="primary" @click="addDialogTip = false">确 定</el-button>
        </span>
      </el-dialog>
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
      dialogTableVisible: false,
      dialogFormVisible: false,
      addDialogTip: false,
      update_dialog_visible: false,
      curItem: {
        name: "",
        id: -1
      },
      form: {
        name: ""
      },
      formLabelWidth: "120px",
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
          var recvData = response.data.data;
          recvData = JSON.parse(recvData);
          var list = recvData["list"];
          var tableData = [];
          for (var i = 0; i < list.length; i++) {
            var obj = list[i];
            var newObj = {};
            newObj["dictype_id"] = obj["dictype_id"];
            newObj["index"] = i;
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
    },
    deleteDictype(index) {},
    updateDictype(index) {
      console.log("lin=updateDictype=" + index);
      this.update_dialog_visible = true;
      var obj = this.tableData[index];
      this.curItem.name = obj["dictype_name"];
      this.curItem.id = obj["dictype_id"];
    },
    updateDictypeSummit() {
      var req_data = {};
      req_data["type"] = 1;
      req_data["dictpe_name"] = "'" + this.form.name + "'";
      this.$http({
        method: "get",
        url: "/api/operatorDicType"
      })
        .then(response => {
          var recvData = response.data.data;
          recvData = JSON.parse(recvData);
          var list = recvData["list"];
          var tableData = [];
          for (var i = 0; i < list.length; i++) {
            var obj = list[i];
            var newObj = {};
            newObj["dictype_id"] = obj["dictype_id"];
            newObj["index"] = i;
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
    },
    addDictype() {
      if (this.form.name == "") return;
      this.dialogFormVisible = false;
      var req_data = {};
      req_data["type"] = 1;
      req_data["dictpe_name"] = "'" + this.form.name + "'";
      this.$http({
        method: "get",
        params: req_data,
        url: "/api/operatorDicType"
      })
        .then(response => {
          this.addDialogTip = true;
          this.getTableData();

          var recvData = response.data.data;
          recvData = JSON.parse(recvData);
          var list = recvData["list"];
        })
        .catch(function(err) {
          console.log(err);
        });
    }
  }
};
</script>
