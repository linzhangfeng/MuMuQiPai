<template>
  <div class="page3">
    我是page3
  </div>
 
</template>

<script>
  export default {
    name: 'page3',
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
