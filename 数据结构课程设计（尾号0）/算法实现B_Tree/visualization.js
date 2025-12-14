// B-树可视化类
class BTreeVisualization {
    constructor(containerId, btree = null) {
        this.container = document.getElementById(containerId);
        this.tree = btree || new BTree(); // 使用传入的btree或创建新的
        this.highlightedNodes = new Set();
        this.animationSpeed = 500;
    }

    // 渲染整个树
    async renderTree(highlightKey = null) {
        const structure = await this.tree.getTreeStructure();
        
        if (!structure) {
            this.renderEmptyTree();
            return;
        }

        this.container.innerHTML = '';
        this.container.className = 'tree-visualization';
        
        // 创建SVG容器用于绘制连接线
        const svgContainer = this.createElement('div', 'svg-container');
        const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
        svg.setAttribute('class', 'connection-svg');
        svgContainer.appendChild(svg);
        
        const treeDiv = this.createElement('div', 'tree-content');
        
        const levels = this._groupByLevel(structure);
        const nodePositions = new Map(); // 存储节点位置信息
        
        levels.forEach((levelNodes, levelIndex) => {
            const levelDiv = this.createElement('div', 'btree-level');
            levelDiv.style.marginTop = levelIndex > 0 ? '60px' : '20px';
            
            levelNodes.forEach((node, nodeIndex) => {
                const nodeElement = this.createNodeElement(node, highlightKey);
                nodeElement.setAttribute('data-level', levelIndex);
                nodeElement.setAttribute('data-index', nodeIndex);
                levelDiv.appendChild(nodeElement);
                
                // 存储节点位置信息
                const nodeId = `${levelIndex}-${nodeIndex}`;
                nodePositions.set(nodeId, {
                    element: nodeElement,
                    node: node,
                    level: levelIndex,
                    index: nodeIndex
                });
            });
            
            treeDiv.appendChild(levelDiv);
        });
        
        this.container.appendChild(svgContainer);
        this.container.appendChild(treeDiv);

        // 等待DOM更新后绘制连接线，增加延迟确保布局完成
        setTimeout(async () => {
            // 强制重新计算布局
            this.container.offsetHeight;
            await this.drawConnectionLines(svg, nodePositions, levels);
        }, 100);
    }

    // 渲染空树
    renderEmptyTree() {
        this.container.innerHTML = `
            <div class="empty-tree">
                <p>🌳</p>
                <p>空树</p>
                <p>请插入数据开始构造B-树</p>
            </div>
        `;
        this.container.className = 'tree-visualization';
    }

    // 创建DOM元素
    createElement(tag, className = '', textContent = '') {
        const element = document.createElement(tag);
        if (className) element.className = className;
        if (textContent) element.textContent = textContent;
        return element;
    }

    // 创建节点元素
    createNodeElement(nodeData, highlightKey = null) {
        const nodeDiv = this.createElement('div', 'btree-node');
        
        // 检查是否包含高亮的键
        let hasHighlightKey = false;
        if (highlightKey !== null && nodeData.keys.includes(highlightKey)) {
            hasHighlightKey = true;
            nodeDiv.classList.add('highlight');
        }
        
        const keysDiv = this.createElement('div', 'btree-keys');
        
        nodeData.keys.forEach(key => {
            const keyDiv = this.createElement('div', 'btree-key', key.toString());
            
            // 如果是要高亮的键，添加特殊样式
            if (key === highlightKey) {
                keyDiv.classList.add('found');
            }
            
            keysDiv.appendChild(keyDiv);
        });
        
        nodeDiv.appendChild(keysDiv);
        
        // 添加节点信息（调试用）
        nodeDiv.title = `层级: ${nodeData.level}, 键: [${nodeData.keys.join(', ')}], ${nodeData.isLeaf ? '叶子' : '内部'}节点`;
        
        return nodeDiv;
    }

    // 按层级分组节点
    _groupByLevel(structure) {
        const levels = [];
        
        const traverse = (node) => {
            if (!levels[node.level]) {
                levels[node.level] = [];
            }
            levels[node.level].push(node);
            
            node.children.forEach(child => traverse(child));
        };
        
        traverse(structure);
        return levels;
    }

    // 绘制连接线
    async drawConnectionLines(svg, nodePositions, levels) {
        svg.innerHTML = ''; // 清空之前的线条
        
        // 设置SVG尺寸
        const containerRect = this.container.getBoundingClientRect();
        svg.setAttribute('width', containerRect.width);
        svg.setAttribute('height', containerRect.height);
        svg.style.position = 'absolute';
        svg.style.top = '0';
        svg.style.left = '0';
        svg.style.pointerEvents = 'none';
        svg.style.zIndex = '1';
        
        const containerOffset = this.container.getBoundingClientRect();
        
        // 获取完整的树结构
        const structure = await this.tree.getTreeStructure();
        if (!structure) return;
        
        // 使用新的连接线绘制方法
        this._drawConnectionsWithLevelOrder(svg, structure, nodePositions, containerOffset);
    }

    // 新的连接线绘制方法：按层级顺序绘制
    _drawConnectionsWithLevelOrder(svg, rootNode, nodePositions, containerOffset) {
        const queue = [{node: rootNode, level: 0}];
        
        while (queue.length > 0) {
            const {node, level} = queue.shift();
            
            // 如果不是叶子节点且有子节点，绘制连接线
            if (!node.isLeaf && node.children && node.children.length > 0) {
                const parentElement = this._findNodeElementByKeys(node.keys, level, nodePositions);
                
                if (parentElement) {
                    const parentRect = parentElement.getBoundingClientRect();
                    const parentCenterX = parentRect.left + parentRect.width / 2 - containerOffset.left;
                    const parentBottomY = parentRect.bottom - containerOffset.top;
                    
                    // 为每个子节点绘制连接线
                    node.children.forEach(child => {
                        if (child) {
                            const childElement = this._findNodeElementByKeys(child.keys, level + 1, nodePositions);
                            
                            if (childElement) {
                                const childRect = childElement.getBoundingClientRect();
                                const childCenterX = childRect.left + childRect.width / 2 - containerOffset.left;
                                const childTopY = childRect.top - containerOffset.top;
                                
                                // 创建连接线
                                const line = document.createElementNS('http://www.w3.org/2000/svg', 'line');
                                line.setAttribute('x1', parentCenterX);
                                line.setAttribute('y1', parentBottomY);
                                line.setAttribute('x2', childCenterX);
                                line.setAttribute('y2', childTopY);
                                line.setAttribute('stroke', '#6c757d');
                                line.setAttribute('stroke-width', '2');
                                line.setAttribute('stroke-linecap', 'round');
                                
                                svg.appendChild(line);
                            }
                            
                            // 将子节点加入队列
                            queue.push({node: child, level: level + 1});
                        }
                    });
                }
            }
        }
    }

    // 通过键值和层级查找节点元素
    _findNodeElementByKeys(keys, level, nodePositions) {
        for (const [key, position] of nodePositions) {
            if (position.node.level === level && 
                position.node.keys.length === keys.length &&
                position.node.keys.every((k, i) => k === keys[i])) {
                return position.element;
            }
        }
        return null;
    }

    _findNodeElement(nodeData, nodePositions) {
        // 首先尝试通过nodeId匹配（最准确）
        if (nodeData.nodeId) {
            for (const [key, position] of nodePositions) {
                if (position.node.nodeId === nodeData.nodeId) {
                    return position.element;
                }
            }
        }
        
        // 如果没有nodeId，则通过键值匹配
        for (const [key, position] of nodePositions) {
            if (this._compareNodes(position.node, nodeData)) {
                return position.element;
            }
        }
        return null;
    }

    // 辅助方法：比较两个节点是否相同
    _compareNodes(node1, node2) {
        // 首先比较nodeId（如果存在）
        if (node1.nodeId && node2.nodeId) {
            return node1.nodeId === node2.nodeId;
        }
        
        // 比较键值数组
        if (node1.keys.length !== node2.keys.length) return false;
        for (let i = 0; i < node1.keys.length; i++) {
            if (node1.keys[i] !== node2.keys[i]) return false;
        }
        
        // 比较层级
        if (node1.level !== node2.level) return false;
        
        return true;
    }

    // 插入动画
    async animateInsert(key) {
        const result = await this.tree.insert(key);
        
        // 重新渲染树，高亮新插入的键
        await this.renderTree(result.success ? key : null);
        
        // 强制重绘连接线确保正确显示
        if (result.success) {
            setTimeout(async () => {
                await this.forceRedrawConnections();
            }, 150);
            
            // 添加插入成功的动画效果
            setTimeout(async () => {
                await this.renderTree(); // 移除高亮
            }, 2000);
        }
        
        return result;
    }

    // 查找动画
    async animateSearch(key) {
        const result = await this.tree.search(key);
        
        if (result.found) {
            // 高亮找到的键
            await this.renderTree(key);
            
            // 2秒后移除高亮
            setTimeout(async () => {
                await this.renderTree();
            }, 2000);
        } else {
            // 显示查找失败
            await this.renderTree();
        }
        
        return result;
    }

    // 删除动画
    async animateDelete(key) {
        // 先查找是否存在
        const searchResult = await this.tree.search(key);
        if (!searchResult.found) {
            return { success: false, message: `键 ${key} 不存在` };
        }

        // 短暂高亮要删除的键
        await this.renderTree(key);
        
        // 等待一下再执行删除
        await this.delay(1000);
        
        const result = await this.tree.delete(key);
        
        // 重新渲染树
        await this.renderTree();
        
        // 强制重绘连接线
        setTimeout(async () => {
            await this.forceRedrawConnections();
        }, 150);
        
        return result;
    }

    // 清空树
    async clear() {
        await this.tree.clear();
        this.renderEmptyTree();
    }

    // 获取树的统计信息
    getTreeStats() {
        return {
            depth: this.tree.getDepth(),
            nodeCount: this.tree.getNodeCount(),
            keyCount: this.tree.getKeyCount()
        };
    }

    // 获取操作历史
    getHistory() {
        return this.tree.operationHistory;
    }

    // 清空历史
    clearHistory() {
        this.tree.clearHistory();
    }

    // 辅助方法：延迟
    delay(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }

    // 检查树是否达到最小深度要求（至少为3）
    async checkDepthRequirement() {
        const depth = await this.tree.getDepth();
        return {
            currentDepth: depth,
            meetsRequirement: depth >= 3,
            message: depth >= 3 ? 
                `✅ 树的深度为 ${depth}，满足最小深度3的要求` : 
                `⚠️ 树的深度为 ${depth}，还需要插入更多数据才能达到最小深度3`
        };
    }

    // 生成随机测试数据
    generateRandomData(count = 15) {
        const data = new Set();
        while (data.size < count) {
            data.add(Math.floor(Math.random() * 100) + 1);
        }
        return Array.from(data).sort((a, b) => a - b);
    }

    // 延迟方法
    delay(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }

    // 强制重绘连接线
    async forceRedrawConnections() {
        const svg = this.container.querySelector('.connection-svg');
        if (svg) {
            // 重新获取节点位置
            const nodeElements = this.container.querySelectorAll('.btree-node');
            const nodePositions = new Map();
            
            nodeElements.forEach((element, index) => {
                const level = parseInt(element.getAttribute('data-level') || '0');
                const nodeIndex = parseInt(element.getAttribute('data-index') || '0');
                const nodeId = `${level}-${nodeIndex}`;
                
                // 获取节点的键值用于匹配
                const keys = Array.from(element.querySelectorAll('.btree-key')).map(keyEl => parseInt(keyEl.textContent));
                
                nodePositions.set(nodeId, {
                    element: element,
                    node: { keys, level, isLeaf: !element.classList.contains('internal') },
                    level: level,
                    index: nodeIndex
                });
            });

            const containerOffset = this.container.getBoundingClientRect();
            const structure = await this.tree.getTreeStructure();
            
            if (structure) {
                this._drawConnectionsWithLevelOrder(svg, structure, nodePositions, containerOffset);
            }
        }
    }
}
