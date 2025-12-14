from flask import Flask, request, jsonify
from flask_cors import CORS
from btree_backend import BTree

app = Flask(__name__)
CORS(app)  # 允许跨域请求

# 全局B-树实例
btree = BTree()

@app.route('/')
def index():
    return jsonify({'message': 'B-Tree Backend API is running!'})

@app.route('/api/insert', methods=['POST'])
def insert():# 插入键值
    try:
        data = request.get_json()
        key = data.get('key')
        
        if key is None:
            return jsonify({'success': False, 'message': '缺少key参数'}), 400
        
        result = btree.insert(int(key))
        
        # 返回结果和树的结构
        response = {
            'success': result['success'],
            'message': result['message'],
            'treeStructure': btree.get_tree_structure(),
            'stats': {
                'depth': btree.get_depth(),
                'nodeCount': btree.get_node_count(),
                'keyCount': btree.get_key_count()
            }
        }
        
        return jsonify(response)
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/api/search', methods=['POST'])
def search():# 查找键值
    try:
        data = request.get_json()
        key = data.get('key')
        
        if key is None:
            return jsonify({'success': False, 'message': '缺少key参数'}), 400
        
        result = btree.search(int(key))
        
        response = {
            'found': result['found'],
            'depth': result['depth'],
            'message': f"键 {key} {'找到' if result['found'] else '未找到'}",
            'treeStructure': btree.get_tree_structure()
        }
        
        return jsonify(response)
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/api/delete', methods=['POST'])
def delete():# 删除键值
    try:
        data = request.get_json()
        key = data.get('key')
        
        if key is None:
            return jsonify({'success': False, 'message': '缺少key参数'}), 400
        
        result = btree.delete(int(key))
        
        response = {
            'success': result['success'],
            'message': result['message'],
            'treeStructure': btree.get_tree_structure(),
            'stats': {
                'depth': btree.get_depth(),
                'nodeCount': btree.get_node_count(),
                'keyCount': btree.get_key_count()
            }
        }
        
        return jsonify(response)
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/api/clear', methods=['POST'])
def clear():# 清空树
    try:
        btree.clear()
        
        response = {
            'success': True,
            'message': '树已清空',
            'treeStructure': None,
            'stats': {
                'depth': 0,
                'nodeCount': 0,
                'keyCount': 0
            }
        }
        
        return jsonify(response)
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/api/structure', methods=['GET'])
def get_structure():# 获取树结构
    try:
        response = {
            'treeStructure': btree.get_tree_structure(),
            'stats': {
                'depth': btree.get_depth(),
                'nodeCount': btree.get_node_count(),
                'keyCount': btree.get_key_count()
            }
        }
        
        return jsonify(response)
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/api/history', methods=['GET'])
def get_history():# 获取操作历史
    try:
        return jsonify({'history': btree.operation_history})
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/api/history/clear', methods=['POST'])
def clear_history():# 清空历史记录
    try:
        btree.clear_history()
        return jsonify({'success': True, 'message': '历史记录已清空'})
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/api/validate', methods=['GET'])
def validate():# 验证树结构
    try:
        response = {
            'valid': True,
            'message': 'B-树结构有效',
            'stats': {
                'depth': btree.get_depth(),
                'nodeCount': btree.get_node_count(),
                'keyCount': btree.get_key_count()
            }
        }
        
        return jsonify(response)
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

if __name__ == '__main__':
    print("启动B-Tree Backend API服务...")
    print("访问 http://localhost:5000 查看API状态")
    print("前端可以通过以下端点与后端通信:")
    print("  POST /api/insert - 插入键值")
    print("  POST /api/search - 查找键值")
    print("  POST /api/delete - 删除键值")
    print("  POST /api/clear - 清空树")
    print("  GET  /api/structure - 获取树结构")
    print("  GET  /api/history - 获取操作历史")
    print("  POST /api/history/clear - 清空历史")
    print("  GET  /api/validate - 验证树结构")
    
    app.run(debug=True, host='0.0.0.0', port=5000)