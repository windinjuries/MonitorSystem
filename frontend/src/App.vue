<template>
  <div id="app">
    <div class="container">
      <!-- Header -->
      <div class="header">
        <h1>系统监控</h1>
        <p>实时监控系统性能指标</p>
      </div>

      <!-- System Info Cards -->
      <div class="card-grid">
        <!-- Time Card -->
        <div class="card">
          <div class="card-header">
            <span class="card-title">系统时间</span>
            <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <circle cx="12" cy="12" r="10"></circle>
              <polyline points="12 6 12 12 16 14"></polyline>
            </svg>
          </div>
          <div class="card-value">{{ time }}</div>
        </div>

        <!-- CPU Card -->
        <div class="card">
          <div class="card-header">
            <span class="card-title">CPU 使用率</span>
            <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <rect x="4" y="4" width="16" height="16" rx="2" ry="2"></rect>
              <rect x="9" y="9" width="6" height="6"></rect>
              <line x1="9" y1="1" x2="9" y2="4"></line>
              <line x1="15" y1="1" x2="15" y2="4"></line>
              <line x1="9" y1="20" x2="9" y2="23"></line>
              <line x1="15" y1="20" x2="15" y2="23"></line>
              <line x1="20" y1="9" x2="23" y2="9"></line>
              <line x1="20" y1="14" x2="23" y2="14"></line>
              <line x1="1" y1="9" x2="4" y2="9"></line>
              <line x1="1" y1="14" x2="4" y2="14"></line>
            </svg>
          </div>
          <div class="progress-ring-container">
            <div
              class="progress-ring"
              :style="{ background: `conic-gradient(#3b82f6 ${cpuProgress}%, #e5e7eb ${cpuProgress}%)` }"
            >
              <div class="progress-ring-value">{{ cpu }}</div>
            </div>
          </div>
        </div>

        <!-- Memory Card -->
        <div class="card">
          <div class="card-header">
            <span class="card-title">内存使用率</span>
            <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M6 19v-3"></path>
              <path d="M10 19v-3"></path>
              <path d="M14 19v-3"></path>
              <path d="M18 19v-3"></path>
              <path d="M8 11V9"></path>
              <path d="M16 11V9"></path>
              <path d="M12 11V9"></path>
              <path d="M2 15h20"></path>
              <path d="M2 7a2 2 0 0 1 2-2h16a2 2 0 0 1 2 2v1.1a2 2 0 0 0 0 3.837V17a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2v-5.1a2 2 0 0 0 0-3.837Z"></path>
            </svg>
          </div>
          <div class="progress-ring-container">
            <div
              class="progress-ring"
              :style="{ background: `conic-gradient(#10b981 ${memoryProgress}%, #e5e7eb ${memoryProgress}%)` }"
            >
              <div class="progress-ring-value">{{ memory }}</div>
            </div>
          </div>
        </div>
      </div>

      <!-- Terminal Section -->
      <div class="terminal-section">
        <h2>Bash 命令终端</h2>
        <textarea
          v-model="BashInput"
          class="terminal-input"
          placeholder="输入 bash 命令..."
          rows="4"
        ></textarea>
        <button @click="execute_bash" class="execute-btn">执行命令</button>
        <textarea
          v-model="ResultOutput"
          class="terminal-output"
          placeholder="命令执行结果..."
          rows="12"
          readonly
        ></textarea>
      </div>
    </div>
  </div>
</template>

<style scoped>
/* Component-specific styles are in main.css */
</style>

<script>
import axios from '@/js/axios.js'
import formatTimestamp from '@/js/utils.js'

export default {
  mounted() {
    this.getmachineinfo();
    this.timer = setInterval(() => {
      setTimeout(() => {
        this.getmachineinfo();
      }, 0)
    }, 2000);
  },
  data() {
    return {
      time: '1970-1-1 00:00:00',
      cpu: '0%',
      memory: '0%',
      BashInput: '',
      ResultOutput: ''
    }
  },
  computed: {
    cpuProgress() {
      return parseFloat(this.cpu) || 0;
    },
    memoryProgress() {
      return parseFloat(this.memory) || 0;
    }
  },
  methods: {
    getmachineinfo() {
      axios.get('/api/info')
        .then(response => {
          this.time = formatTimestamp(response.time);
          this.cpu = response.cpu.toFixed(1) + '%';
          this.memory = response.memory.toFixed(1) + '%';
        })
        .catch(error => {
          console.error('There was an error!', error);
        });
    },
    execute_bash() {
      let request = { "script": this.BashInput };
      axios.post('/api/bash', request)
        .then(response => {
          this.ResultOutput = response.result;
        })
        .catch(error => {
          console.error('There was an error!', error);
          this.ResultOutput = 'Error: ' + error.message;
        });
    }
  },
  beforeUnmount() {
    if (this.timer) {
      clearInterval(this.timer);
    }
  }
}
</script>