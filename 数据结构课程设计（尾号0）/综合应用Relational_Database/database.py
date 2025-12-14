from flask import Flask, jsonify, request, render_template_string
from flask_cors import CORS
import json

app = Flask(__name__)
CORS(app)

class Node:
    def __init__(self, id, data, pid=0):
        self.id = id
        self.data = data
        self.pid = pid
        self.children = []
    
    def to_dict(self):#将节点转换为字典，便于JSON序列化
        return {
            'id': self.id,
            'data': self.data,
            'pid': self.pid,
            'children': [child.to_dict() for child in self.children]
        }

class TreeDatabase:#模拟关系数据库表存储树结构
    def __init__(self):
        # 模拟数据库表：存储所有节点的线性表
        self.table = []
        # 用于快速查找的索引
        self.id_index = {}
        # 自增ID计数器
        self.next_id = 1
    
    def add_node_to_table(self, data, pid=0):#向线性表中添加节点
        node_data = {
            'id': self.next_id,
            'data': data,
            'pid': pid
        }
        self.table.append(node_data)
        self.id_index[self.next_id] = node_data
        self.next_id += 1
        return node_data['id']
    
    def remove_node_from_table(self, node_id):#从线性表中删除节点及其子节点
        if node_id not in self.id_index:
            return False
        
        # 找到所有子节点
        children_ids = [node['id'] for node in self.table if node['pid'] == node_id]
        
        # 递归删除所有子节点
        for child_id in children_ids:
            self.remove_node_from_table(child_id)
        
        # 删除当前节点
        node_data = self.id_index[node_id]
        self.table.remove(node_data)
        del self.id_index[node_id]
        return True
    
    def update_node_parent(self, node_id, new_pid):#更新节点的父节点
        if node_id in self.id_index:
            self.id_index[node_id]['pid'] = new_pid
            return True
        return False
    
    def linear_to_tree(self):

        if not self.table:
            return None
        
        # 创建节点映射
        nodes = {}
        root = None
        
        # 第一遍遍历：创建所有节点
        for record in self.table:
            node = Node(record['id'], record['data'], record['pid'])
            nodes[record['id']] = node
            
            # 找到根节点（pid为0）
            if record['pid'] == 0:
                root = node
        
        # 第二遍遍历：建立父子关系
        for record in self.table:
            if record['pid'] != 0 and record['pid'] in nodes:
                parent = nodes[record['pid']]
                child = nodes[record['id']]
                parent.children.append(child)
        
        return root
    
    def insert_tree_node(self, parent_id, data):
        """
        算法2：在树中插入节点，自动更新线性表
        """
        # 验证父节点是否存在（如果不是根节点）
        if parent_id != 0 and parent_id not in self.id_index:
            return None
        
        # 向线性表中添加新节点
        new_id = self.add_node_to_table(data, parent_id)
        return new_id
    
    def delete_tree_node(self, node_id):
        """
        算法3：删除树中的节点，自动更新线性表
        """
        return self.remove_node_from_table(node_id)
    
    def get_table_data(self):
        """获取线性表数据"""
        return self.table.copy()
    
    def get_tree_data(self):
        """获取树结构数据"""
        root = self.linear_to_tree()
        return root.to_dict() if root else None

# 全局数据库实例
db = TreeDatabase()

# 初始化一些测试数据
db.add_node_to_table("根节点", 0)
db.add_node_to_table("子节点1", 1)
db.add_node_to_table("子节点2", 1)
db.add_node_to_table("子节点1.1", 2)
db.add_node_to_table("子节点1.2", 2)
db.add_node_to_table("子节点2.1", 3)

@app.route('/')
def index():
    """主页面"""
    return render_template_string(HTML_TEMPLATE)

@app.route('/api/table', methods=['GET'])
def get_table():
    """获取线性表数据"""
    return jsonify(db.get_table_data())

@app.route('/api/tree', methods=['GET'])
def get_tree():
    """获取树结构数据"""
    return jsonify(db.get_tree_data())

@app.route('/api/node', methods=['POST'])
def add_node():
    """添加新节点"""
    data = request.json
    parent_id = data.get('parent_id', 0)
    node_data = data.get('data', '')
    
    new_id = db.insert_tree_node(parent_id, node_data)
    if new_id:
        return jsonify({'success': True, 'id': new_id})
    else:
        return jsonify({'success': False, 'error': '父节点不存在'})

@app.route('/api/node/<int:node_id>', methods=['DELETE'])
def delete_node(node_id):
    """删除节点"""
    success = db.delete_tree_node(node_id)
    return jsonify({'success': success})

# HTML模板
HTML_TEMPLATE = '''
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>关系数据库树结构模拟器</title>
    <style>
        body {
            font-family: 'Microsoft YaHei', Arial, sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #f5f5f5;
        }
        
        .container {
            max-width: 1400px;
            margin: 0 auto;
            background: white;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        
        h1 {
            text-align: center;
            color: #333;
            margin-bottom: 30px;
        }
        
        .panel {
            display: flex;
            gap: 20px;
            margin-bottom: 20px;
        }
        
        .section {
            flex: 1;
            border: 1px solid #ddd;
            border-radius: 8px;
            padding: 15px;
            background: #fafafa;
        }
        
        .section h3 {
            margin-top: 0;
            color: #666;
            border-bottom: 2px solid #007ACC;
            padding-bottom: 5px;
        }
        
        .controls {
            margin-bottom: 20px;
            padding: 15px;
            background: #e8f4fd;
            border-radius: 8px;
        }
        
        .control-group {
            display: flex;
            gap: 10px;
            align-items: center;
            margin-bottom: 10px;
        }
        
        input, select, button {
            padding: 8px 12px;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 14px;
        }
        
        button {
            background: #007ACC;
            color: white;
            cursor: pointer;
            border: none;
        }
        
        button:hover {
            background: #005a9e;
        }
        
        .delete-btn {
            background: #dc3545;
        }
        
        .delete-btn:hover {
            background: #c82333;
        }
        
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 10px;
        }
        
        th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
        }
        
        th {
            background: #f0f0f0;
            font-weight: bold;
        }
        
        .tree-container {
            background: white;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 15px;
            min-height: 300px;
            font-family: monospace;
        }
        
        .tree-node {
            margin: 5px 0;
            padding: 5px 10px;
            background: #f8f9fa;
            border: 1px solid #dee2e6;
            border-radius: 4px;
            position: relative;
        }
        
        .tree-node.root {
            background: #d4edda;
            border-color: #c3e6cb;
        }
        
        .tree-children {
            margin-left: 30px;
            border-left: 2px solid #dee2e6;
            padding-left: 10px;
        }
        
        .node-info {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        
        .node-actions {
            display: flex;
            gap: 5px;
        }
        
        .btn-small {
            padding: 4px 8px;
            font-size: 12px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>关系数据库树结构模拟器</h1>
        
        <div class="controls">
            <h3>操作控制</h3>
            <div class="control-group">
                <label>节点数据：</label>
                <input type="text" id="nodeData" placeholder="输入节点数据">
                <label>父节点ID：</label>
                <select id="parentSelect">
                    <option value="0">根节点 (0)</option>
                </select>
                <button onclick="addNode()">添加节点</button>
                <button onclick="refreshData()">刷新数据</button>
            </div>
        </div>
        
        <div class="panel">
            <div class="section">
                <h3>线性表结构 (模拟关系数据库表)</h3>
                <div id="tableView">
                    <table>
                        <thead>
                            <tr>
                                <th>ID</th>
                                <th>数据</th>
                                <th>父节点ID (PID)</th>
                                <th>操作</th>
                            </tr>
                        </thead>
                        <tbody id="tableBody">
                        </tbody>
                    </table>
                </div>
            </div>
            
            <div class="section">
                <h3>树形结构</h3>
                <div class="tree-container" id="treeView">
                </div>
            </div>
        </div>
    </div>

    <script>
        // 页面加载时获取数据
        window.onload = function() {
            refreshData();
        };
        
        // 刷新数据
        function refreshData() {
            fetchTableData();
            fetchTreeData();
            updateParentSelect();
        }
        
        // 获取线性表数据
        function fetchTableData() {
            fetch('/api/table')
                .then(response => response.json())
                .then(data => {
                    renderTable(data);
                })
                .catch(error => {
                    console.error('获取表格数据失败:', error);
                });
        }
        
        // 获取树结构数据
        function fetchTreeData() {
            fetch('/api/tree')
                .then(response => response.json())
                .then(data => {
                    renderTree(data);
                })
                .catch(error => {
                    console.error('获取树结构数据失败:', error);
                });
        }
        
        // 渲染线性表
        function renderTable(data) {
            const tbody = document.getElementById('tableBody');
            tbody.innerHTML = '';
            
            data.forEach(node => {
                const row = document.createElement('tr');
                row.innerHTML = `
                    <td>${node.id}</td>
                    <td>${node.data}</td>
                    <td>${node.pid}</td>
                    <td>
                        <button class="delete-btn btn-small" onclick="deleteNode(${node.id})">删除</button>
                    </td>
                `;
                tbody.appendChild(row);
            });
        }
        
        // 渲染树结构
        function renderTree(treeData) {
            const treeContainer = document.getElementById('treeView');
            
            if (!treeData) {
                treeContainer.innerHTML = '<p>暂无数据</p>';
                return;
            }
            
            treeContainer.innerHTML = renderTreeNode(treeData, true);
        }
        
        // 递归渲染树节点
        function renderTreeNode(node, isRoot = false) {
            let html = `
                <div class="tree-node ${isRoot ? 'root' : ''}">
                    <div class="node-info">
                        <span><strong>ID:</strong> ${node.id} | <strong>数据:</strong> ${node.data} | <strong>PID:</strong> ${node.pid}</span>
                        <div class="node-actions">
                            <button class="btn-small" onclick="addChildNode(${node.id})">添加子节点</button>
                            ${!isRoot ? `<button class="delete-btn btn-small" onclick="deleteNode(${node.id})">删除</button>` : ''}
                        </div>
                    </div>
            `;
            
            if (node.children && node.children.length > 0) {
                html += '<div class="tree-children">';
                node.children.forEach(child => {
                    html += renderTreeNode(child, false);
                });
                html += '</div>';
            }
            
            html += '</div>';
            return html;
        }
        
        // 更新父节点选择器
        function updateParentSelect() {
            fetch('/api/table')
                .then(response => response.json())
                .then(data => {
                    const select = document.getElementById('parentSelect');
                    select.innerHTML = '<option value="0">根节点 (0)</option>';
                    
                    data.forEach(node => {
                        const option = document.createElement('option');
                        option.value = node.id;
                        option.textContent = `${node.data} (${node.id})`;
                        select.appendChild(option);
                    });
                });
        }
        
        // 添加节点
        function addNode() {
            const nodeData = document.getElementById('nodeData').value;
            const parentId = parseInt(document.getElementById('parentSelect').value);
            
            if (!nodeData.trim()) {
                alert('请输入节点数据');
                return;
            }
            
            fetch('/api/node', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    data: nodeData,
                    parent_id: parentId
                })
            })
            .then(response => response.json())
            .then(result => {
                if (result.success) {
                    document.getElementById('nodeData').value = '';
                    refreshData();
                    alert('节点添加成功！');
                } else {
                    alert('添加失败: ' + result.error);
                }
            })
            .catch(error => {
                console.error('添加节点失败:', error);
                alert('添加节点失败');
            });
        }
        
        // 添加子节点
        function addChildNode(parentId) {
            const nodeData = prompt('请输入子节点数据:');
            if (nodeData) {
                fetch('/api/node', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({
                        data: nodeData,
                        parent_id: parentId
                    })
                })
                .then(response => response.json())
                .then(result => {
                    if (result.success) {
                        refreshData();
                        alert('子节点添加成功！');
                    } else {
                        alert('添加失败: ' + result.error);
                    }
                });
            }
        }
        
        // 删除节点
        function deleteNode(nodeId) {
            if (confirm('确定要删除这个节点吗？这将同时删除其所有子节点。')) {
                fetch(`/api/node/${nodeId}`, {
                    method: 'DELETE'
                })
                .then(response => response.json())
                .then(result => {
                    if (result.success) {
                        refreshData();
                        alert('节点删除成功！');
                    } else {
                        alert('删除失败');
                    }
                })
                .catch(error => {
                    console.error('删除节点失败:', error);
                    alert('删除节点失败');
                });
            }
        }
    </script>
</body>
</html>
'''

if __name__ == '__main__':
    print("关系数据库树结构模拟器启动中...")
    print("算法功能说明：")
    print("1. 线性表转树结构：O(n)时间复杂度将关系数据库表转换为树")
    print("2. 树中插入节点：自动更新线性表结构")
    print("3. 树中删除节点：级联删除子节点并更新线性表")
    print("\n服务器启动后请访问: http://127.0.0.1:5000")
    app.run(debug=True, port=5000)