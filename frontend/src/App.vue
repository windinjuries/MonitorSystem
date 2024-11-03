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
      <h1>time: {{ time }}</h1>
      <h1>cpu usage: {{ cpu }}</h1>
      <h1>memory usage: {{ memory }}</h1>
      <textarea v-model="BashInput" placeholder="" rows="4" cols="80"></textarea>
      <button @click="execute_bash">send</button>
      <textarea v-model="ResultOutput" placeholder="" rows="12" cols="80"></textarea>
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
    /* display: flex;
    place-items: flex-start;
    flex-wrap: wrap; */
  }
}
</style>

<script>
export default {
  mounted() {
    this.getmachineinfo();
    this.timer = setInterval(() => {
      setTimeout(() => {
        this.getmachineinfo() //调用接口的方法
      }, 0)
    }, 2000);
  },
  data() {
    return {
      time: '1970-1-1 00:00:00',
      cpu: '0%',
      memory: '0%'
    }
  },
  methods: {
    getmachineinfo() {
      axios.get('/api/info')
        .then(response => {
          console.log(response.time)
          this.time = formatTimestamp(response.time);
          this.cpu = response.cpu;
          this.memory = response.memory;
        })
        .catch(error => {
          console.error('There was an error!', error);
        });
    },  
    execute_bash() {
    console.log("click")
    let request = {"script":""};
    let script = this.BashInput;
    request.script = script;
    axios.post('/api/bash', request) 
        .then(response => {
          console.log(response.result);
          this.ResultOutput = response.result;
          this.$forceUpdate();
        })
        .catch(error => {
          console.error('There was an error!', error);
        });
    }
  }
}
</script>