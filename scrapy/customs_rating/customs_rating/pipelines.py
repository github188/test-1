# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html

import json
import psycopg2

class CustomsRatingPipeline(object):
    comm = None
    cur = None
    def __init__(self):
        """
        连接数据库，并建立Cursor对象
        """
        self.conn = psycopg2.connect(database="customs_rating", user="postgres", password="postgres", host="127.0.0.1", port="5432")
        self.cur = self.conn.cursor()
        print("~~~~ CONNECT DATABASE SUCCESS ~~~~")

    def process_item(self, item, spider):
        print(item['company'][0].strip().strip('\''), type(item['company'][0].strip()))
        print(item['ratings'][0].strip(), type(item['ratings'][0].strip()))
        print(item['pages'], type(item['pages']))
        self.cur.execute("""INSERT INTO customs_rating(company_name, rating, pages)
                VALUES('%s', '%s', %d)"""
                %(item['company'][0].strip().strip('\''), item['ratings'][0].strip(), item['pages']))
        self.conn.commit()
        return item
