# B-树Python实现 (3阶B-树/2-3树)
import json
from datetime import datetime

class BTreeNode:
    def __init__(self, is_leaf=True):
        self.keys = []           # 键值数组
        self.children = []       # 子节点数组
        self.is_leaf = is_leaf   # 是否为叶子节点
        self.parent = None       # 父节点

    def is_full(self):# 检查节点是否已满（键数量达到上限）
        return len(self.keys) >= 2

    def needs_merge(self):# 检查节点是否需要合并（键数量低于下限）
        return len(self.keys) < 1

    def find_key(self, key):# 在节点中查找键，返回索引或-1
        for i, k in enumerate(self.keys):
            if k == key:
                return i
        return -1

    def find_insert_position(self, key):# 找到插入键的位置
        i = 0
        while i < len(self.keys) and self.keys[i] < key:
            i += 1
        return i

    def insert_key(self, key):# 插入键值到节点
        pos = self.find_insert_position(key)
        self.keys.insert(pos, key)

    def remove_key(self, key):# 删除键值
        if key in self.keys:
            self.keys.remove(key)
            return True
        return False

    def split(self):# 分裂节点
        if len(self.keys) < 3:
            raise ValueError('节点键数量不足，无法分裂')
        
        mid = 1  # 3阶B-树中间位置固定是1
        mid_key = self.keys[mid]
        
        # 创建新的右节点
        right_node = BTreeNode(self.is_leaf)
        right_node.keys = self.keys[mid + 1:]  # 右节点包含中间键右边的所有键
        
        # 如果不是叶子节点，也要分割子节点
        if not self.is_leaf:
            right_node.children = self.children[mid + 1:]  # 右节点包含中间键右边的所有子节点
            # 更新子节点的父节点引用
            for child in right_node.children:
                if child:
                    child.parent = right_node
        
        # 更新当前节点（左节点）
        self.keys = self.keys[:mid]  # 左节点只保留中间键左边的键
        if not self.is_leaf:
            self.children = self.children[:mid + 1]  # 左节点保留中间键左边的子节点+1
        
        return mid_key, right_node

class BTree:
    def __init__(self):
        self.root = None
        self.operation_history = []

    def add_to_history(self, operation, key, result, details=''):# 添加操作到历史记录
        timestamp = datetime.now().strftime('%H:%M:%S')
        self.operation_history.append({
            'operation': operation,
            'key': key,
            'result': result,
            'details': details,
            'timestamp': timestamp
        })

    def clear_history(self):
        self.operation_history = []

    def get_depth(self):
        if not self.root:
            return 0
        return self._get_node_depth(self.root)

    def _get_node_depth(self, node):
        if not node or node.is_leaf:
            return 1
        max_depth = 0
        for child in node.children:
            if child:
                max_depth = max(max_depth, self._get_node_depth(child))
        return max_depth + 1

    def get_node_count(self):
        if not self.root:
            return 0
        return self._count_nodes(self.root)

    def _count_nodes(self, node):
        if not node:
            return 0
        count = 1
        for child in node.children:
            if child:
                count += self._count_nodes(child)
        return count

    def get_key_count(self):
        if not self.root:
            return 0
        return self._count_keys(self.root)

    def _count_keys(self, node):
        if not node:
            return 0
        count = len(node.keys)
        for child in node.children:
            if child:
                count += self._count_keys(child)
        return count

    def search(self, key):
        result = self._search(self.root, key)
        self.add_to_history('search', key, result['found'], 
                           f"在深度 {result['depth']} 找到" if result['found'] else '未找到')
        return result

    def _search(self, node, key):
        if not node:
            return {'found': False, 'node': None, 'depth': 0, 'path': []}

        # 在当前节点中查找
        key_index = node.find_key(key)
        if key_index != -1:
            return {
                'found': True,
                'node': node,
                'keyIndex': key_index,
                'depth': 1,
                'path': [node]
            }

        # 如果是叶子节点，查找失败
        if node.is_leaf:
            return {'found': False, 'node': None, 'depth': 1, 'path': [node]}

        # 在子节点中查找
        child_index = node.find_insert_position(key)
        child_result = self._search(node.children[child_index], key)
        
        return {
            'found': child_result['found'],
            'node': child_result['node'],
            'keyIndex': child_result.get('keyIndex'),
            'depth': child_result['depth'] + 1,
            'path': [node] + child_result['path']
        }

    def insert(self, key):
        # 检查是否已存在
        search_result = self._search(self.root, key)
        if search_result['found']:
            self.add_to_history('insert', key, False, '键已存在')
            return {'success': False, 'message': f'键 {key} 已存在'}

        if not self.root:
            self.root = BTreeNode(True)
            self.root.insert_key(key)
            self.add_to_history('insert', key, True, '创建根节点')
            return {'success': True, 'message': f'成功插入 {key}（根节点）'}

        result = self._insert(self.root, key)
        if result['success']:
            self.add_to_history('insert', key, True, result.get('details', '插入成功'))
        else:
            self.add_to_history('insert', key, False, result['message'])
        return result

    def _insert(self, node, key):
        if node.is_leaf:
            # 叶子节点直接插入
            node.insert_key(key)
            
            if len(node.keys) > 2:
                # 需要分裂
                return self._split_node(node, key)
            
            return {'success': True, 'message': f'成功插入 {key}'}

        # 内部节点，找到合适的子节点
        child_index = node.find_insert_position(key)
        child = node.children[child_index]
        
        result = self._insert(child, key)
        
        if result.get('needs_split'):
            # 子节点分裂了，需要提升中间键
            return self._handle_child_split(node, child_index, result['mid_key'], result['right_node'])
        
        return result

    def _split_node(self, node, inserted_key):
        if len(node.keys) <= 2:
            return {'success': True, 'message': f'插入 {inserted_key} 成功'}

        mid_key, right_node = node.split()

        if not node.parent:
            # 如果是根节点分裂，创建新的根节点
            new_root = BTreeNode(False)
            new_root.keys.append(mid_key)
            new_root.children.extend([node, right_node])
            
            node.parent = new_root
            right_node.parent = new_root
            
            self.root = new_root
            
            return {
                'success': True,
                'message': f'插入 {inserted_key} 导致根节点分裂，树高度增加',
                'details': '根节点分裂'
            }

        return {
            'success': True,
            'needs_split': True,
            'mid_key': mid_key,
            'right_node': right_node,
            'message': f'插入 {inserted_key} 导致节点分裂'
        }

    def _handle_child_split(self, parent, child_index, mid_key, right_node):
        # 将中间键插入到父节点
        parent.insert_key(mid_key)
        
        # 插入新的子节点
        parent.children.insert(child_index + 1, right_node)
        right_node.parent = parent

        # 检查父节点是否需要分裂
        if len(parent.keys) > 2:
            return self._split_node(parent, mid_key)

        return {'success': True, 'message': '分裂处理完成'}

    def delete(self, key):
        if not self.root:
            self.add_to_history('delete', key, False, '树为空')
            return {'success': False, 'message': '树为空'}

        result = self._delete(self.root, key)
        if result['success']:
            self.add_to_history('delete', key, True, result.get('details', '删除成功'))
            
            # 检查根节点状态
            if len(self.root.keys) == 0:
                if self.root.is_leaf:
                    # 根节点是叶子节点且为空，树变为空
                    self.root = None
                elif self.root.children:
                    # 根节点为内部节点且为空，提升唯一子节点为新根
                    self.root = self.root.children[0]
                    if self.root:
                        self.root.parent = None
        else:
            self.add_to_history('delete', key, False, result['message'])
        return result

    def _delete(self, node, key):
        key_index = node.find_key(key)
        
        if key_index != -1:
            # 在当前节点找到键
            if node.is_leaf:
                # 叶子节点直接删除
                node.remove_key(key)
                return self._check_underflow(node, key)
            else:
                # 内部节点，需要找到前驱或后继来替换
                return self._delete_from_internal_node(node, key_index, key)
        else:
            # 键不在当前节点
            if node.is_leaf:
                return {'success': False, 'message': f'键 {key} 不存在'}
            
            # 在子节点中查找并删除
            child_index = node.find_insert_position(key)
            return self._delete(node.children[child_index], key)

    def _delete_from_internal_node(self, node, key_index, key):
        # 从左子树找前驱
        left_child = node.children[key_index]
        predecessor = self._find_max(left_child)
        
        # 用前驱替换当前键
        node.keys[key_index] = predecessor
        
        # 删除前驱
        return self._delete(left_child, predecessor)

    def _find_max(self, node):
        while not node.is_leaf:
            node = node.children[-1]
        return node.keys[-1]

    def _check_underflow(self, node, deleted_key):
        if node == self.root:
            return {'success': True, 'message': f'成功删除 {deleted_key}'}

        if len(node.keys) >= 1:
            return {'success': True, 'message': f'成功删除 {deleted_key}'}

        # 发生下溢，需要重新平衡
        return self._rebalance(node, deleted_key)

    def _rebalance(self, node, deleted_key):
        parent = node.parent
        node_index = parent.children.index(node)
        
        # 尝试从兄弟节点借用
        left_sibling = parent.children[node_index - 1] if node_index > 0 else None
        right_sibling = parent.children[node_index + 1] if node_index < len(parent.children) - 1 else None
        
        # 从左兄弟借用
        if left_sibling and len(left_sibling.keys) > 1:
            borrowed_key = left_sibling.keys.pop()
            parent_key = parent.keys[node_index - 1]
            parent.keys[node_index - 1] = borrowed_key
            node.keys.insert(0, parent_key)
            
            if not left_sibling.is_leaf:
                borrowed_child = left_sibling.children.pop()
                node.children.insert(0, borrowed_child)
                if borrowed_child:
                    borrowed_child.parent = node
            
            return {'success': True, 'message': f'删除 {deleted_key}，从左兄弟借用键重新平衡'}
        
        # 从右兄弟借用
        if right_sibling and len(right_sibling.keys) > 1:
            borrowed_key = right_sibling.keys.pop(0)
            parent_key = parent.keys[node_index]
            parent.keys[node_index] = borrowed_key
            node.keys.append(parent_key)
            
            if not right_sibling.is_leaf:
                borrowed_child = right_sibling.children.pop(0)
                node.children.append(borrowed_child)
                if borrowed_child:
                    borrowed_child.parent = node
            
            return {'success': True, 'message': f'删除 {deleted_key}，从右兄弟借用键重新平衡'}
        
        # 合并节点
        if left_sibling:
            return self._merge(left_sibling, node, node_index - 1, deleted_key)
        elif right_sibling:
            return self._merge(node, right_sibling, node_index, deleted_key)
        
        return {'success': True, 'message': f'删除 {deleted_key}'}

    def _merge(self, left_node, right_node, parent_key_index, deleted_key):
        parent = left_node.parent
        
        # 将父节点的键拉下来
        parent_key = parent.keys[parent_key_index]
        left_node.keys.append(parent_key)
        
        # 合并右节点的键和子节点
        left_node.keys.extend(right_node.keys)
        left_node.children.extend(right_node.children)
        
        # 更新子节点的父节点引用
        for child in right_node.children:
            if child:
                child.parent = left_node
        
        # 从父节点删除键和子节点
        parent.keys.pop(parent_key_index)
        parent.children.pop(parent_key_index + 1)
        
        # 检查父节点是否需要重新平衡
        if parent != self.root and len(parent.keys) < 1:
            return self._rebalance(parent, deleted_key)
        
        return {'success': True, 'message': f'删除 {deleted_key}，合并节点完成重新平衡'}

    def clear(self):
        self.root = None
        self.add_to_history('clear', None, True, '清空整个树')

    def get_tree_structure(self):
        if not self.root:
            return None
        return self._build_structure(self.root, 0)

    def _build_structure(self, node, level):
        import random
        structure = {
            'keys': node.keys.copy(),
            'level': level,
            'isLeaf': node.is_leaf,
            'children': [],
            'nodeId': f"{level}-{random.randint(100000, 999999)}" # 唯一ID
        }

        if not node.is_leaf and node.children:
            for child in node.children:
                if child:
                    structure['children'].append(self._build_structure(child, level + 1))

        return structure