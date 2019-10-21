<template>
  <div class="home">
    <el-row class="tac">
      <el-col :span="4">
        <el-menu
          default-active="0"
          class="el-menu-vertical-demo"
          background-color="#545c64"
          text-color="#fff"
          @open="handleOpen"
          @close="handleClose"
          :default-openeds="['0']"
          active-text-color="#ffd04b"
        >
          <template v-for="item in items">
            <el-submenu v-bind:key="item.id" :index="item.id">
              <template slot="title">
                <i class="el-icon-location"></i>
                <span>{{item.name}}</span>
              </template>
              <el-menu-item
                v-for="child in item.childs"
                v-bind:key="child.id"
                :index="child.id"
                @click="pageClick(child.id)"
              >{{child.name}}</el-menu-item>
            </el-submenu>
          </template>
        </el-menu> 
      </el-col>
      <el-col :span="20" class="content-wrapper">
          <router-view></router-view>
      </el-col>
    </el-row>
  </div>
</template>

<style>
.el-menu-vertical-demo:not(.el-menu--collapse) {
  width: 200px;
  min-height: 1000px;
}
</style>

<script>
export default {
  name: "home",
  data() {
    return {
      openeds: [0],
      items: [
        {
          id: "0",
          name: "数据统计",
          childs: [
            { id: "0_0", name: "用户信息" },
            { id: "0_1", name: "选项二" },
            { id: "0_2", name: "选项三" }
          ]
        },
        {
          id: "1",
          name: "导航二",
          childs: [
            { id: "1_0", name: "选项一" },
            { id: "1_1", name: "选项二" },
            { id: "1_2", name: "选项三" }
          ]
        },
        {
          id: "2",
          name: "导航三",
          childs: [
            { id: "2_0", name: "选项一" },
            { id: "2_1", name: "选项二" },
            { id: "2_2", name: "选项三" }
          ]
        }
      ]
    };
  },
  methods: {
    handleOpen(key, keyPath) {
      console.log(key, keyPath);
    },
    handleClose(key, keyPath) {
      console.log(key, keyPath);
    },
    pageClick(v) {
      console.log("lin=pageClick=" + v);
      if (v == "0_0") {
        this.$router.push({ path: "/UserInfo" });
      } else if (v == "0_1") {
        this.$router.push({ path: "/Page2" });
      } else if (v == "0_2") {
        this.$router.push({ path: "/Page3" });
      }
    }
  }
};
</script>
