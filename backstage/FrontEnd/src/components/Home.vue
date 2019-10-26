<template>
  <el-container style="height: 500px; border: 1px solid #eee">
    <el-aside width="200px" style="background-color: rgb(238, 241, 246)">
      <el-menu :default-openeds="['1']">
        <el-submenu v-for="item in navigationData" v-bind:key="item.tag" :index="item.tag">
          <template slot="title">
            <i :class="item.icon"></i>
            {{item.name}}
          </template>
          <el-menu-item
            v-for="child in item.childs"
            v-bind:key="child.tag"
            :index="child.tag"
            @click="pageClick(child.tag)"
          >{{child.name}}</el-menu-item>
        </el-submenu>
      </el-menu>
    </el-aside>

    <el-container>
      <el-header style="text-align: right; font-size: 12px">
        <el-dropdown>
          <i class="el-icon-setting" style="margin-right: 15px"></i>
          <el-dropdown-menu slot="dropdown">
            <el-dropdown-item>查看</el-dropdown-item>
            <el-dropdown-item>新增</el-dropdown-item>
            <el-dropdown-item>删除</el-dropdown-item>
          </el-dropdown-menu>
        </el-dropdown>
        <span>王小虎</span>
      </el-header>

      <el-main>
        <router-view></router-view>
      </el-main>
    </el-container>
  </el-container>
</template>

<style>
.el-header {
  background-color: #b3c0d1;
  color: #333;
  line-height: 60px;
}

.el-aside {
  color: #333;
}
</style>

<script>
var Navigation = {};
Navigation.OnLineOperation = {
  UserInfo: "UserInfo",
  RoomManage: "RoomManage",
  UserFeedback: "UserFeedback"
};
Navigation.ProductManager = {
  GameCategroy: "GameCategroy",
  GameConfig: "GameConfig"
};
Navigation.Management = {
  ManagerInfo: "ManagerInfo",
  OperatorLog: "OperatorLog",
  NavigatManager: "NavigatManager"
};
Navigation.HelpDocument = {
  DicType: "DicType",
  Dictionary: "Dictionary"
};
export default {
  data() {
    const item = {
      date: "2016-05-02",
      name: "王小虎",
      address: "上海市普陀区金沙江路 1518 弄"
    };
    var items = [
      {
        tag: "1",
        name: "产品管理",
        icon: "el-icon-star-off",
        childs: [
          { name: "游戏分类", tag: Navigation.ProductManager.GameCategroy },
          { name: "游戏配置", tag: Navigation.ProductManager.GameConfig }
        ]
      },
      {
        tag: "2",
        icon: "el-icon-star-on",
        name: "运营数据",
        childs: [
          { name: "用户信息", tag: Navigation.OnLineOperation.UserInfo },
          { name: "房间管理", tag: Navigation.OnLineOperation.RoomManage },
          { name: "用户反馈", tag: Navigation.OnLineOperation.UserFeedback }
        ]
      },
      {
        tag: "3",
        icon: "el-icon-star-on",
        name: "权限管理",
        childs: [
          { name: "管理员账户", tag: Navigation.Management.ManagerInfo },
          { name: "菜单栏管理", tag: Navigation.Management.NavigatManager },
          { name: "操作日记", tag: Navigation.Management.OperatorLog }
        ]
      },
      {
        tag: "4",
        icon: "el-icon-star-on",
        name: "帮助文档",
        childs: [
          { name: "字典查询", tag: Navigation.HelpDocument.Dictionary },
          { name: "字典类型管理", tag: Navigation.HelpDocument.DicType }
        ]
      }
    ];

    return {
      tableData: Array(20).fill(item),
      navigationData: items
    };
  },
  methods: {
    pageClick(v) {
      console.log("lin=v=" + v);
      switch (v) {
        case Navigation.OnLineOperation.UserInfo: {
          this.$router.push({ path: "/UserInfo" });
          break;
        }
        case Navigation.OnLineOperation.RoomManage: {
          break;
        }
        case Navigation.OnLineOperation.UserFeedback: {
          break;
        }
        case Navigation.ProductManager.GameCategroy: {
          this.$router.push({ path: "/GameGategory" });
          break;
        }
        case Navigation.ProductManager.GameConfig: {
          break;
        }

        case Navigation.Management.ManagerInfo: {
          break;
        }

        case Navigation.Management.OperatorLog: {
          break;
        }

        case Navigation.Management.NavigatManager: {
          break;
        }
        case Navigation.HelpDocument.DicType: {
          this.$router.push({ path: "/DicType" });
          break;
        }
        case Navigation.HelpDocument.Dictionary: {
          this.$router.push({ path: "/Dictionary" });
          break;
        }
      }
    }
  }
};
</script>
