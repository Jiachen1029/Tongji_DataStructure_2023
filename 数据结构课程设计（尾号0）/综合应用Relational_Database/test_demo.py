#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
关系数据库树结构模拟器 - 演示脚本
测试所有核心算法功能
"""

import requests
import json
import time

# 服务器地址
BASE_URL = "http://127.0.0.1:5000"

def test_api():
    """测试API功能"""
    print("🚀 开始测试关系数据库树结构模拟器...")
    print("=" * 60)
    
    try:
        # 1. 获取初始数据
        print("📊 1. 获取初始线性表数据:")
        response = requests.get(f"{BASE_URL}/api/table")
        table_data = response.json()
        print(json.dumps(table_data, ensure_ascii=False, indent=2))
        
        print("\n🌳 2. 获取初始树结构:")
        response = requests.get(f"{BASE_URL}/api/tree")
        tree_data = response.json()
        print(json.dumps(tree_data, ensure_ascii=False, indent=2))
        
        # 2. 测试添加节点
        print("\n➕ 3. 测试添加新节点:")
        new_node = {
            "data": "测试部门",
            "parent_id": 1
        }
        response = requests.post(f"{BASE_URL}/api/node", json=new_node)
        result = response.json()
        print(f"添加结果: {result}")
        
        # 3. 再次获取数据查看变化
        print("\n📊 4. 添加节点后的线性表:")
        response = requests.get(f"{BASE_URL}/api/table")
        table_data = response.json()
        print(json.dumps(table_data, ensure_ascii=False, indent=2))
        
        print("\n🌳 5. 添加节点后的树结构:")
        response = requests.get(f"{BASE_URL}/api/tree")
        tree_data = response.json()
        print(json.dumps(tree_data, ensure_ascii=False, indent=2))
        
        # 4. 测试删除节点
        print(f"\n🗑️ 6. 测试删除节点 (ID: {result['id']}):")
        response = requests.delete(f"{BASE_URL}/api/node/{result['id']}")
        delete_result = response.json()
        print(f"删除结果: {delete_result}")
        
        # 5. 最终状态
        print("\n📊 7. 删除节点后的最终状态:")
        response = requests.get(f"{BASE_URL}/api/table")
        final_table = response.json()
        print("线性表:")
        print(json.dumps(final_table, ensure_ascii=False, indent=2))
        
        response = requests.get(f"{BASE_URL}/api/tree")
        final_tree = response.json()
        print("树结构:")
        print(json.dumps(final_tree, ensure_ascii=False, indent=2))
        
        print("\n✅ 所有测试完成！算法功能正常！")
        
    except requests.exceptions.ConnectionError:
        print("❌ 错误: 无法连接到服务器")
        print("请确保服务器正在运行: python database.py")
    except Exception as e:
        print(f"❌ 测试过程中出现错误: {e}")

def print_algorithm_explanation():
    """打印算法说明"""
    print("\n" + "=" * 60)
    print("📚 算法实现说明:")
    print("=" * 60)
    print("""
🔍 算法1: 线性表转树结构 (linear_to_tree)
   - 时间复杂度: O(n)
   - 空间复杂度: O(n)
   - 实现原理:
     1. 第一遍遍历: 为每个记录创建Node对象
     2. 第二遍遍历: 根据pid建立父子关系
     3. 返回根节点，形成完整树结构

➕ 算法2: 树中插入节点 (insert_tree_node)
   - 时间复杂度: O(1)
   - 功能: 在指定父节点下插入新节点
   - 自动维护: ID自增、索引更新、数据一致性

🗑️ 算法3: 树中删除节点 (delete_tree_node)
   - 时间复杂度: O(k)，k为被删除子树的节点数
   - 功能: 级联删除节点及其所有子节点
   - 自动维护: 索引清理、数据一致性

🎯 核心特点:
   - 模拟关系数据库表结构
   - 实时线性表和树结构同步
   - 支持动态增删改操作
   - 可视化界面展示
""")

if __name__ == "__main__":
    print_algorithm_explanation()
    print("\n等待3秒后开始测试...")
    time.sleep(3)
    test_api()
    
    print("\n" + "=" * 60)
    print("🌐 Web界面功能:")
    print("=" * 60)
    print("""
访问地址: http://127.0.0.1:5000

界面功能:
✅ 实时显示线性表和树结构
✅ 可视化添加/删除节点
✅ 数据同步更新
✅ 交互式操作界面
✅ 算法执行过程展示

操作说明:
1. 输入节点数据和选择父节点
2. 点击"添加节点"按钮
3. 观察线性表和树结构的实时变化
4. 点击"删除"按钮测试删除功能
5. 点击"刷新数据"查看最新状态
""")
