<script setup>
import HelloWorld from './components/HelloWorld.vue'
import TheWelcome from './components/TheWelcome.vue'
import axios from '@/js/axios.js'
import formatTimestamp from '@/js/utils.js'
</script>

<template>
  <header>
    <img alt="Vue logo" class="logo" src="./assets/logo.svg" width="125" height="125" />
    <div class="wrapper">
      <h1>{{ message }}</h1> 
      <h2>cpu usage: {{ cpu }}"\n"</h2>

      <h2>memory usage: {{ cpu }}</h2>
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
  mounted() {
    this.gettime();
    this.timer = setInterval(() => {
      setTimeout(() => {
        this.gettime() //调用接口的方法
      }, 0)
    }, 2000)
  },
  data() {
    return {
      message: 'Hello World!',
      cpu: '0%'
    }
  },
  methods: {
    gettime() {
      axios.get('/api/info')
        .then(response => {
          console.log(response.time)
          this.message = formatTimestamp(response.time);
          console.log(this.message)
        })
        .catch(error => {
          console.error('There was an error!', error);
        });
    }
  },

}
</script>