// B-Tree API 通信模块
class BTreeAPI {
    constructor() {
        this.baseURL = 'http://localhost:5000/api';
    }

    async request(endpoint, method = 'GET', data = null) {
        const options = {
            method: method,
            headers: {
                'Content-Type': 'application/json',
            }
        };

        if (data) {
            options.body = JSON.stringify(data);
        }

        try {
            const response = await fetch(`${this.baseURL}${endpoint}`, options);
            const result = await response.json();
            
            if (!response.ok) {
                throw new Error(result.message || `HTTP error! status: ${response.status}`);
            }
            
            return result;
        } catch (error) {
            console.error('API请求错误:', error);
            throw new Error(`网络错误: ${error.message}`);
        }
    }

    async insert(key) {
        return await this.request('/insert', 'POST', { key });
    }

    async search(key) {
        return await this.request('/search', 'POST', { key });
    }

    async delete(key) {
        return await this.request('/delete', 'POST', { key });
    }

    async clear() {
        return await this.request('/clear', 'POST');
    }

    async getStructure() {
        return await this.request('/structure', 'GET');
    }

    async getHistory() {
        return await this.request('/history', 'GET');
    }

    async clearHistory() {
        return await this.request('/history/clear', 'POST');
    }

    async validate() {
        return await this.request('/validate', 'GET');
    }
}

// 模拟原有的BTree类接口，但实际调用后端API
class BTree {
    constructor() {
        this.api = new BTreeAPI();
        this.operationHistory = [];
    }

    async insert(key) {
        try {
            const result = await this.api.insert(key);
            this.updateLocalHistory();
            return result;
        } catch (error) {
            return { success: false, message: error.message };
        }
    }

    async search(key) {
        try {
            const result = await this.api.search(key);
            this.updateLocalHistory();
            return result;
        } catch (error) {
            return { found: false, message: error.message };
        }
    }

    async delete(key) {
        try {
            const result = await this.api.delete(key);
            this.updateLocalHistory();
            return result;
        } catch (error) {
            return { success: false, message: error.message };
        }
    }

    async clear() {
        try {
            const result = await this.api.clear();
            this.updateLocalHistory();
            return result;
        } catch (error) {
            return { success: false, message: error.message };
        }
    }

    async getTreeStructure() {
        try {
            const result = await this.api.getStructure();
            return result.treeStructure;
        } catch (error) {
            console.error('获取树结构失败:', error);
            return null;
        }
    }

    async getDepth() {
        try {
            const result = await this.api.getStructure();
            return result.stats.depth;
        } catch (error) {
            return 0;
        }
    }

    async getNodeCount() {
        try {
            const result = await this.api.getStructure();
            return result.stats.nodeCount;
        } catch (error) {
            return 0;
        }
    }

    async getKeyCount() {
        try {
            const result = await this.api.getStructure();
            return result.stats.keyCount;
        } catch (error) {
            return 0;
        }
    }

    async clearHistory() {
        try {
            await this.api.clearHistory();
            this.operationHistory = [];
        } catch (error) {
            console.error('清空历史失败:', error);
        }
    }

    async updateLocalHistory() {
        try {
            const result = await this.api.getHistory();
            this.operationHistory = result.history;
        } catch (error) {
            console.error('更新历史记录失败:', error);
        }
    }

    async validate() {
        try {
            return await this.api.validate();
        } catch (error) {
            return { valid: false, message: error.message };
        }
    }
}
