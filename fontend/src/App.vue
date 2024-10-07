<script setup>
import HelloWorld from './components/HelloWorld.vue'
import TheWelcome from './components/TheWelcome.vue'
</script>

<template>
  <header>
    <img alt="Vue logo" class="logo" src="./assets/logo.svg" width="125" height="125" />
    <div class="wrapper">
      <button @click="sayHello">gettime</button>
    </div>
    
    <div class="wrapper">
      <h2>获取设备时间：{{message}}</h2>
    </div>
  </header>
  <!-- <main>
    <TheWelcome />
  </main> -->
</template>

<style scoped>
header {
  line-height: 1.5;
}

.logo {
  display: block;
  margin: 0 auto 2rem;
}

@media (min-width: 1024px) {
  header {
    display: flex;
    place-items: center;
    padding-right: calc(var(--section-gap) / 2);
  }

  .logo {
    margin: 0 2rem 0 0;
  }

  header .wrapper {
    display: flex;
    place-items: flex-start;
    flex-wrap: wrap;
  }
}
</style>

<script>
export default {
  data() {
    return {
      message: 'Hello World!'
    }
  }
  methods: {
    gettime() {
      axios.get('/api/info')
        .then(response => {
          this.data = response.data;
        })
        .catch(error => {
          console.error('There was an error!', error);
        });
    }
  }
}
</script>