<template>
  <div class="page1">
    我是page1
  </div>
 
</template>

<script>
  export default {
    name: 'page1',
    methods: {
      handleOpen(key, keyPath) {
        console.log(key, keyPath);
      },
      handleClose(key, keyPath) {
        console.log(key, keyPath);
      }
    }
  }
</script>
