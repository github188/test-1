#-*- coding: utf-8 -*-

import time

import scrapy
import psycopg2

from scrapy.http import  Request
from scrapy.http import  FormRequest

from customs_rating.items import CustomsRatingItem

class customs_rating_spider(scrapy.Spider):
    name = 'customs_rating'
    #allow_domain = 
    #allowed_domains = ["218.57.139.24"]
    start_urls = [
            "http://www.haiguan.info/onLineSearch/IndustryInfo/default.aspx",
    ]
    comm = None
    cur = None
    
    def __init__(self):
        """
        连接数据库，并建立Cursor对象
        """
        self.conn = psycopg2.connect(database="customs_rating", user="postgres", password="postgres", host="127.0.0.1", port="5432")
        self.cur = self.conn.cursor()
        print("!~~~~ CONNECT DATABASE SUCCESS ~~~~!")

    def parse_pages(self, response):
        body = response.request.body.decode('unicode_escape')
        index = body.find("__EVENTARGUMENT")
        eventargument = body[index + 16:index + 36]
        print(eventargument)
        index = eventargument.find("&")
        eventargument = eventargument[0:index]
        pages = int(eventargument)
        print("~~~~~~~~~~~~~parse %d~~~~~~~~~~~~~~~" % pages)
        for sel in response.xpath("/html/body[@id='newyear']/form[@id='aspnetForm']/div[@id='containter']/div[5]/div[@class='box960']/div[@class='right_box']/div[@class='tongguan f_left']/div[@class='qy_nr blue']/ul/li"):
            item = CustomsRatingItem()
            item['company'] = sel.xpath("div[1]/h2/a/text()").extract()
            item['ratings'] = sel.xpath("div[2]/text()").extract()
            item['pages'] = pages
            yield item
        print("~~~~~~~~~~~~~parse %d~~~~~~~~~~~~~~~" % pages)

    def parse(self, response):
        print("~~~~~~~~~~~~~~~~~~~~~ parse ~~~~~~~~~~~~~~~~~~~~~")

        viewstate_value = "/wEPDwUJNjQ1ODMwNzI1D2QWAmYPZBYCAgEPZBYEZg8PFgIeB1Zpc2libGVoZGQCAg9kFggCBA8PFgIeBFRleHQFLuWFseaQnOe0ouaciSAxMTYyODgxIOadoe+8jOavj+mhteaYvuekuiAxMCDmnaFkZAIFDxAPFgYeDURhdGFUZXh0RmllbGQFCGNvX2NsYXNzHg5EYXRhVmFsdWVGaWVsZAUIY29fY2xhc3MeC18hRGF0YUJvdW5kZ2QQFQkG5omA5pyJAAABLgFBAkFBAUIBQwFEFQkAAAABLgFBAkFBAUIBQwFEFCsDCWdnZ2dnZ2dnZxYBZmQCBg8WAh4LXyFJdGVtQ291bnQCChYUZg9kFgJmDxUFCjA3MDM5OTAwMDEpTFrlhoXokpnlj6Toh6rmsrvljLrmlofnianogIPlj6TnoJTnqbbmiYAKMDcwMzk5MDAwMQABQmQCAQ9kFgJmDxUFCjA4MDAwMTA1OTgh5rKI6Ziz6YeR5p2v6L+b5Ye65Y+j5pyJ6ZmQ5YWs5Y+4CjA4MDAwMTA1OTgAAUJkAgIPZBYCZg8VBQowOTAwOTkwMDAxLExa5Lit5Zu956eR5a2m6Zmi5aSn6L+e5YyW5a2m54mp55CG56CU56m25omACjA5MDA5OTAwMDEAAUJkAgMPZBYCZg8VBQoxMDM3OTEyMDEyKuW8oOWutuWPo+W4guS6muWMl+WvueWklue7j+a1jui0uOaYk+WFrOWPuAoxMDM3OTEyMDEyAAFCZAIED2QWAmYPFQUKMTEwMDAxOTAwMi3kuK3lm73nlLXmsJTov5vlh7rlj6PogZTokKXlhazlj7jlpKnmtKXlhazlj7gKMTEwMDAxOTAwMgABQmQCBQ9kFgJmDxUFCjExMDAwOTAwMDAb5YyX5Lqs5b635rqQ55S15Zmo5Z+O5YWs5Y+4CjExMDAwOTAwMDAAAUJkAgYPZBYCZg8VBQoxMTAwOTEwMDY2DOmmlumDveWuvummhgoxMTAwOTEwMDY2AAFCZAIHD2QWAmYPFQUKMTEwMDkxMDEzNSHkuK3ljIXljbDliLfnianotYTov5vlh7rlj6Plhazlj7gKMTEwMDkxMDEzNQABQmQCCA9kFgJmDxUFCjExMDEwMTk5OTkq5Zub5bed5r6c5rW35ZOB5oKm5Zu96ZmF6LS45piT5pyJ6ZmQ5YWs5Y+4CjExMDEwMTk5OTkAAUJkAgkPZBYCZg8VBQoxMTAxMDc2MDAxHuWMl+S6rOeJueWugeenkeaKgOaciemZkOWFrOWPuAoxMTAxMDc2MDAxAAFCZAIIDw8WAh4LUmVjb3JkY291bnQCgf1GZGQYAQUeX19Db250cm9sc1JlcXVpcmVQb3N0QmFja0tleV9fFgEFGGN0bDAwJE1haW5Db250ZW50JEltZ0J0bvYKd4UuIRtsB9Iu5GNhhwdKpnpW+jTHsay4dpqO4toY"
        for sel in response.xpath("//input[@name='__VIEWSTATE']"):
            viewstate_value = sel.xpath("@value").extract()[0]
        
        viewstategenerator_value = "09670E38"
        for sel in response.xpath("//input[@name='__VIEWSTATEGENERATOR']"):
            viewstategenerator_value = sel.xpath("@value").extract()[0]

        #for sel in response.xpath("//input[@name='word1']"):
        #    print(sel.xpath("@value").extract()[0])
        #for sel in response.xpath("//input[@name='ctl00$MainContent$txtRegName']"):
        #    print(sel.xpath("@value").extract()[0])

        form_data = {}
        form_data['__EVENTTARGET'] = 'ctl00$MainContent$AspNetPager1'
        form_data['__LASTFOCUS'] = ''
        form_data['__VIEWSTATE'] = viewstate_value
        form_data['__VIEWSTATEGENERATOR'] = viewstategenerator_value
        form_data['word'] = ''
        form_data['hdSearch'] = 'S1'
        #form_data['word1'] = b'请输入商品名称或编码'.decode('gdk')
        form_data['word1'] = ''
        #form_data['ctl00$MainContent$txtRegName'] = b'请输入要查询的企业名称'.decode('gdk')
        form_data['ctl00$MainContent$txtRegName'] = ''
        form_data['ctl00$MainContent$ddlclass'] = ''
       
        test_count = 0

        print("search customs_rating where pages from 1 to 1162882")

        for i in range(1, 1162882):
            self.cur.execute("SELECT count(id) from customs_rating WHERE pages = %d" % i)
            count_num = self.cur.fetchone()
            
            if i % 100 == 0:
                print("search to %d" % i)

            if count_num[0] != 10:
                test_count += 1
                #if test_count > 10:
                #    exit()
                time.sleep(1)
                print("##### %d -- %d #####" % (test_count, i))
                form_data['ctl00$MainContent$AspNetPager1_input'] = str(i)
                form_data['__EVENTARGUMENT'] = str(i)
            
            #print(form_data)
                yield FormRequest(self.start_urls[0], formdata = form_data, callback = self.parse_pages) 
        
    
    
    #def parse_page(self, response):
        #bbsItem_loader = ItemLoader(item=ScrapyRuanyifengItem(), response = response)
        #url = str(response.url)
        #bbsItem_loader.add_value('url', url)
        #bbsItem_loader.add_value('name', response.xpath("//h1/text()").extract())
        #bbsItem_loader.add_xpath('name', self._x_query['forum'])
        #return bbsItem_loader.load_item()
