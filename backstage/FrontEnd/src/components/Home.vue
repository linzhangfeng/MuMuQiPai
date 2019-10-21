<template>
  <el-container style="height: 500px; border: 1px solid #eee">
    <el-aside width="200px" style="background-color: rgb(238, 241, 246)">
      <el-menu :default-openeds="['1', '3']">
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
  UserInfo:"UserInfo",
  RoomManage:"RoomManage",
  UserFeedback:"UserFeedback",
}
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
          { name: "游戏分类", tag: "1_1" },
          { name: "游戏配置", tag: "1_2" }
        ]
      },
      {
        tag: "2",
        icon: "el-icon-star-on",
        name: "运营数据",
        childs: [
          { name: "用户信息", tag: "2_1" },
          { name: "房间管理", tag: "2_2" },
          { name: "用户反馈", tag: "2_3" }
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
      console.log("lin=v="+v);
      this.$router.push({ path: "/UserInfo" });
    }
  }
};
</script>
