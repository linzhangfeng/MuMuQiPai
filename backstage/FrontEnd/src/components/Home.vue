<template>
  <div class="home">
  <el-row class="tac">
  <el-col :span="12">
    <el-menu
      default-active="2"
      class="el-menu-vertical-demo"
      @open="handleOpen"
      @close="handleClose"
      background-color="#545c64"
      text-color="#fff"
      active-text-color="#ffd04b">
      <el-submenu index="1">
        <template slot="title">
          <i class="el-icon-location"></i>
          <span>导航一</span>
        </template>
        <el-menu-item-group>
            <el-menu-item index="1-1" @click="pageClick(1)">选项1</el-menu-item>
            <el-menu-item index="1-2" @click="pageClick(2)">选项2</el-menu-item>
        </el-menu-item-group>
      </el-submenu>
      <el-menu-item index="2">
        <i class="el-icon-menu"></i>
        <span slot="title">导航二</span>
      </el-menu-item>
      <el-menu-item index="3" disabled>
        <i class="el-icon-document"></i>
        <span slot="title">导航三</span>
      </el-menu-item>
      <el-menu-item index="4">
        <i class="el-icon-setting"></i>
        <span slot="title">导航四</span>
      </el-menu-item>
    </el-menu>
      </el-col>
       <div>
    <router-view ></router-view >
  </div>
</el-row>


  </div>
 
</template>

<script>
  export default {
    name: 'home',
    methods: {
      handleOpen(key, keyPath) {
        console.log(key, keyPath);
      },
      handleClose(key, keyPath) {
        console.log(key, keyPath);
      },
      pageClick(v) {
        console.log("lin=pageClick="+v);
        if(v == 1){
          this.$router.push({ path: '/Page1' })
        }else if(v == 2){
          this.$router.push({ path: '/Page2' })
        }else if(v == 3){
          this.$router.push({ path: '/Page3' })
        }
      }
    }
  }
</script>
