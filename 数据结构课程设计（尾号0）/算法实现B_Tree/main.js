// 主控制脚本
class BTreeApp {
    constructor() {
        this.btree = new BTree(); // 使用新的API B-树
        this.visualization = new BTreeVisualization('treeVisualization', this.btree);
        this.initializeElements();
        this.bindEvents();
        this.updateDisplay();
        this.checkBackendConnection();
    }

    // 检查后端连接
    async checkBackendConnection() {
        try {
            await this.btree.api.request('/', 'GET');
            this.showMessage('✅ 后端连接正常，使用Python B-树实现', 'success');
        } catch (error) {
            this.showMessage('⚠️ 后端连接失败，请确保Python服务已启动 (python app.py)', 'error');
        }
    }

    // 初始化DOM元素引用
    initializeElements() {
        this.elements = {
            valueInput: document.getElementById('valueInput'),
            insertBtn: document.getElementById('insertBtn'),
            searchBtn: document.getElementById('searchBtn'),
            deleteBtn: document.getElementById('deleteBtn'),
            clearBtn: document.getElementById('clearBtn'),
            validateBtn: document.getElementById('validateBtn'),
            messageArea: document.getElementById('messageArea'),
            treeDepth: document.getElementById('treeDepth'),
            nodeCount: document.getElementById('nodeCount'),
            keyCount: document.getElementById('keyCount'),
            historyList: document.getElementById('historyList'),
            clearHistoryBtn: document.getElementById('clearHistoryBtn')
        };
    }

    // 绑定事件处理器
    bindEvents() {
        // 按钮事件
        this.elements.insertBtn.addEventListener('click', () => this.handleInsert());
        this.elements.searchBtn.addEventListener('click', () => this.handleSearch());
        this.elements.deleteBtn.addEventListener('click', () => this.handleDelete());
        this.elements.clearBtn.addEventListener('click', () => this.handleClear());
        this.elements.validateBtn.addEventListener('click', () => this.handleValidate());
        this.elements.clearHistoryBtn.addEventListener('click', () => this.handleClearHistory());

        // 输入框事件
        this.elements.valueInput.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                this.handleInsert();
            }
        });

        // 输入验证
        this.elements.valueInput.addEventListener('input', (e) => {
            this.validateInput(e.target);
        });
    }

    // 验证输入
    validateInput(input) {
        const value = parseInt(input.value);
        const isValid = !isNaN(value) && value >= 1 && value <= 999;
        
        // 更新按钮状态
        this.elements.insertBtn.disabled = !isValid;
        this.elements.searchBtn.disabled = !isValid;
        this.elements.deleteBtn.disabled = !isValid;

        if (input.value && !isValid) {
            input.style.borderColor = '#dc3545';
        } else {
            input.style.borderColor = '#ddd';
        }
    }

    // 获取输入值
    getInputValue() {
        const value = parseInt(this.elements.valueInput.value);
        if (isNaN(value) || value < 1 || value > 999) {
            this.showMessage('请输入1-999之间的数字', 'error');
            return null;
        }
        return value;
    }

    // 清空输入框
    clearInput() {
        this.elements.valueInput.value = '';
        this.validateInput(this.elements.valueInput);
        this.elements.valueInput.focus();
    }

    // 处理插入操作
    async handleInsert() {
        const value = this.getInputValue();
        if (value === null) return;

        this.setButtonsEnabled(false);
        this.showMessage('正在插入...', 'info');

        try {
            const result = await this.visualization.animateInsert(value);
            
            if (result.success) {
                this.showMessage(result.message, 'success');
                this.clearInput();
            } else {
                this.showMessage(result.message, 'error');
            }
        } catch (error) {
            this.showMessage(`插入失败: ${error.message}`, 'error');
        }

        this.updateDisplay();
        this.setButtonsEnabled(true);
    }

    // 处理查找操作
    async handleSearch() {
        const value = this.getInputValue();
        if (value === null) return;

        this.setButtonsEnabled(false);
        this.showMessage('正在查找...', 'info');

        try {
            const result = await this.visualization.animateSearch(value);
            
            if (result.found) {
                this.showMessage(`✅ 找到键 ${value}！位于深度 ${result.depth}`, 'success');
            } else {
                this.showMessage(`❌ 未找到键 ${value}`, 'error');
            }
        } catch (error) {
            this.showMessage(`查找失败: ${error.message}`, 'error');
        }

        this.updateDisplay();
        this.setButtonsEnabled(true);
    }

    // 处理删除操作
    async handleDelete() {
        const value = this.getInputValue();
        if (value === null) return;

        this.setButtonsEnabled(false);
        this.showMessage('正在删除...', 'info');

        try {
            const result = await this.visualization.animateDelete(value);
            
            if (result.success) {
                this.showMessage(result.message, 'success');
                this.clearInput();
            } else {
                this.showMessage(result.message, 'error');
            }
        } catch (error) {
            this.showMessage(`删除失败: ${error.message}`, 'error');
        }

        this.updateDisplay();
        this.setButtonsEnabled(true);
    }

    // 处理清空操作
    async handleClear() {
        if (confirm('确定要清空整个树吗？此操作不可撤销。')) {
            this.setButtonsEnabled(false);
            this.showMessage('正在清空树...', 'info');

            try {
                const result = await this.btree.clear();
                if (result.success) {
                    this.visualization.clear();
                    this.showMessage(result.message, 'success');
                    this.clearInput();
                } else {
                    this.showMessage(result.message, 'error');
                }
            } catch (error) {
                this.showMessage(`清空失败: ${error.message}`, 'error');
            }

            this.updateDisplay();
            this.setButtonsEnabled(true);
        }
    }

    // 处理验证操作
    async handleValidate() {
        this.setButtonsEnabled(false);
        this.showMessage('正在验证树结构...', 'info');

        try {
            const result = await this.btree.validate();
            const stats = await this.getTreeStats();
            
            let message = `🔍 树验证结果:\n`;
            message += `✅ 结构有效: ${result.valid ? '是' : '否'}\n`;
            message += `📏 当前深度: ${stats.depth}\n`;
            message += `🎯 满足深度要求(≥3): ${stats.depth >= 3 ? '是' : '否'}\n`;
            message += `🔢 总键数: ${stats.keyCount}\n`;
            message += `📦 总节点数: ${stats.nodeCount}`;
            
            const type = result.valid && stats.depth >= 3 ? 'success' : 
                        result.valid ? 'info' : 'error';
            
            this.showMessage(message, type);
        } catch (error) {
            this.showMessage(`验证失败: ${error.message}`, 'error');
        }

        this.setButtonsEnabled(true);
    }

    // 处理清空历史
    async handleClearHistory() {
        try {
            await this.btree.clearHistory();
            this.updateHistoryDisplay();
            this.showMessage('操作历史已清空', 'info');
        } catch (error) {
            this.showMessage(`清空历史失败: ${error.message}`, 'error');
        }
    }

    // 显示消息
    showMessage(message, type = 'info') {
        this.elements.messageArea.textContent = message;
        this.elements.messageArea.className = `message-area message-${type}`;
        
        // 3秒后清空消息
        setTimeout(() => {
            if (this.elements.messageArea.textContent === message) {
                this.elements.messageArea.textContent = '';
                this.elements.messageArea.className = 'message-area';
            }
        }, 3000);
    }

    // 设置按钮启用状态
    setButtonsEnabled(enabled) {
        const buttons = [
            this.elements.insertBtn,
            this.elements.searchBtn,
            this.elements.deleteBtn,
            this.elements.clearBtn,
            this.elements.validateBtn
        ];

        buttons.forEach(btn => {
            btn.disabled = !enabled;
        });

        // 输入框在操作期间也禁用
        this.elements.valueInput.disabled = !enabled;
    }

    // 更新显示信息
    async updateDisplay() {
        await this.updateTreeStats();
        await this.updateHistoryDisplay();
        await this.visualization.renderTree();
    }

    // 更新树统计信息
    async updateTreeStats() {
        try {
            const stats = await this.getTreeStats();
            this.elements.treeDepth.textContent = stats.depth;
            this.elements.nodeCount.textContent = stats.nodeCount;
            this.elements.keyCount.textContent = stats.keyCount;
        } catch (error) {
            console.error('更新树统计信息失败:', error);
        }
    }

    // 获取树统计信息的辅助方法
    async getTreeStats() {
        try {
            const [depth, nodeCount, keyCount] = await Promise.all([
                this.btree.getDepth(),
                this.btree.getNodeCount(),
                this.btree.getKeyCount()
            ]);
            return { depth, nodeCount, keyCount };
        } catch (error) {
            return { depth: 0, nodeCount: 0, keyCount: 0 };
        }
    }

    // 更新历史记录显示
    async updateHistoryDisplay() {
        try {
            const listElement = this.elements.historyList;
            const history = this.btree.operationHistory;
            
            listElement.innerHTML = '';

            if (history.length === 0) {
                listElement.innerHTML = '<div class="history-item">暂无操作历史</div>';
                return;
            }

            // 显示最近的操作（倒序）
            const recentHistory = history.slice(-20).reverse();
            
            recentHistory.forEach(entry => {
                const item = document.createElement('div');
                item.className = `history-item ${entry.operation}`;
                
                const operationText = this.getOperationText(entry.operation);
                const resultIcon = entry.result ? '✅' : '❌';
                const keyText = entry.key !== null ? entry.key : '';
                
                item.innerHTML = `
                    <span>${entry.timestamp}</span> - 
                    <strong>${operationText}</strong> 
                    ${keyText} 
                    ${resultIcon} 
                    <em>${entry.details}</em>
                `;
                
                listElement.appendChild(item);
            });
        } catch (error) {
            console.error('更新历史记录显示失败:', error);
        }
    }

    // 获取操作文本
    getOperationText(operation) {
        const operationMap = {
            'insert': '插入',
            'search': '查找',
            'delete': '删除',
            'clear': '清空'
        };
        return operationMap[operation] || operation;
    }
}

// 页面加载完成后初始化应用
document.addEventListener('DOMContentLoaded', () => {
    window.btreeApp = new BTreeApp();
    
    // 显示欢迎消息
    setTimeout(() => {
        btreeApp.showMessage('欢迎使用B-树可视化系统！请输入数字开始操作。', 'info');
    }, 500);
});

// 全局错误处理
window.addEventListener('error', (event) => {
    console.error('应用错误:', event.error);
    if (window.btreeApp) {
        btreeApp.showMessage('应用发生错误，请刷新页面重试', 'error');
    }
});

// 导出供调试使用
if (typeof module !== 'undefined' && module.exports) {
    module.exports = { BTreeApp, BTreeVisualization, BTree };
}
