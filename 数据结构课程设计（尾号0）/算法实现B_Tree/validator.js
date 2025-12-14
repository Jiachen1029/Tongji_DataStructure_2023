// B-树测试和验证脚本
class BTreeValidator {
    constructor(btree) {
        this.btree = btree;
    }

    // 验证B-树的所有属性
    validateTree() {
        if (!this.btree.root) {
            return { valid: true, message: '空树是有效的' };
        }

        const issues = [];
        
        try {
            this._validateNode(this.btree.root, null, null, issues);
            this._validateDepth(this.btree.root, issues);
            this._validateParentChildRelations(this.btree.root, issues);
        } catch (error) {
            issues.push(`验证过程中发生错误: ${error.message}`);
        }

        return {
            valid: issues.length === 0,
            message: issues.length === 0 ? 'B-树结构正确' : `发现 ${issues.length} 个问题`,
            issues: issues
        };
    }

    // 验证单个节点
    _validateNode(node, minKey, maxKey, issues) {
        if (!node) return;

        // 检查键的数量
        if (node.keys.length === 0) {
            if (node !== this.btree.root) {
                issues.push(`非根节点不能为空: 键数量 = ${node.keys.length}`);
            }
        } else if (node.keys.length > 2) {
            issues.push(`节点键数量超过限制: ${node.keys.length} > 2`);
        }

        // 检查键的顺序
        for (let i = 1; i < node.keys.length; i++) {
            if (node.keys[i] <= node.keys[i-1]) {
                issues.push(`节点内键顺序错误: ${node.keys[i-1]} >= ${node.keys[i]}`);
            }
        }

        // 检查键的范围
        if (minKey !== null && node.keys[0] <= minKey) {
            issues.push(`键 ${node.keys[0]} 违反最小值约束 > ${minKey}`);
        }
        if (maxKey !== null && node.keys[node.keys.length - 1] >= maxKey) {
            issues.push(`键 ${node.keys[node.keys.length - 1]} 违反最大值约束 < ${maxKey}`);
        }

        // 检查子节点数量
        if (!node.isLeaf) {
            const expectedChildren = node.keys.length + 1;
            let actualChildren = 0;
            for (let child of node.children) {
                if (child) actualChildren++;
            }
            
            if (actualChildren !== expectedChildren) {
                issues.push(`子节点数量错误: 期望 ${expectedChildren}, 实际 ${actualChildren}`);
            }

            // 递归验证子节点
            for (let i = 0; i < node.children.length; i++) {
                const child = node.children[i];
                if (!child) continue;

                let childMinKey = i === 0 ? minKey : node.keys[i-1];
                let childMaxKey = i === node.keys.length ? maxKey : node.keys[i];

                this._validateNode(child, childMinKey, childMaxKey, issues);
            }
        } else {
            // 叶子节点不应该有子节点
            if (node.children.length > 0) {
                issues.push(`叶子节点不应该有子节点: ${node.children.length}`);
            }
        }
    }

    // 验证所有叶子节点在同一深度
    _validateDepth(node, issues) {
        const leafDepths = [];
        this._collectLeafDepths(node, 0, leafDepths);
        
        const uniqueDepths = [...new Set(leafDepths)];
        if (uniqueDepths.length > 1) {
            issues.push(`叶子节点深度不一致: ${uniqueDepths.join(', ')}`);
        }
    }

    _collectLeafDepths(node, depth, leafDepths) {
        if (!node) return;
        
        if (node.isLeaf) {
            leafDepths.push(depth);
        } else {
            for (let child of node.children) {
                if (child) {
                    this._collectLeafDepths(child, depth + 1, leafDepths);
                }
            }
        }
    }

    // 验证父子关系
    _validateParentChildRelations(node, issues, expectedParent = null) {
        if (!node) return;

        if (node.parent !== expectedParent) {
            issues.push(`父节点引用错误`);
        }

        if (!node.isLeaf) {
            for (let child of node.children) {
                if (child) {
                    this._validateParentChildRelations(child, issues, node);
                }
            }
        }
    }

    // 生成验证报告
    generateReport() {
        const validation = this.validateTree();
        const stats = {
            depth: this.btree.getDepth(),
            nodeCount: this.btree.getNodeCount(),
            keyCount: this.btree.getKeyCount()
        };

        return {
            validation,
            stats,
            summary: {
                isValid: validation.valid,
                meetsDepthRequirement: stats.depth >= 3,
                totalKeys: stats.keyCount,
                totalNodes: stats.nodeCount
            }
        };
    }
}

// 添加到主应用中
if (typeof window !== 'undefined') {
    window.BTreeValidator = BTreeValidator;
}
