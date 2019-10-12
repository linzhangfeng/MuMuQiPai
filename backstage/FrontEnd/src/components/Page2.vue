<template>
  <div class="page2">
    我是page2
  </div>
 
</template>

<script>
  export default {
    name: 'page2',
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
