function formatTimestamp(timestamp, options = {}) {
    console.log(timestamp)
    // 创建 Date 对象
    const date = new Date(timestamp * 1000);

    // 默认选项
    const defaultOptions = {
        year: 'numeric',
        month: '2-digit',
        day: '2-digit',
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit',
        hour12: false // 24小时制
    };

    // 合并用户提供的选项和默认选项
    const finalOptions = { ...defaultOptions, ...options };

    // 返回格式化后的日期字符串
    console.log(date.getSeconds());
    return date.toLocaleString('zh-CN', finalOptions);
}

export default formatTimestamp;
